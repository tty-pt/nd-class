/* src/libnd-class.c — nd-class, ported to libxylem.
 *
 * Owns character classes (fighter, sorcerer, cleric, rogue): their life dice
 * and the per-level HP they add. Co-implements nd-attr's `hp_max` chain.
 *
 * Original: tty-pt/nd-class @ 80 lines main.c, from the nd-basics
 * superproject.
 *
 * This TU XY_IMPLs hp_max, on_add and on_status. It calls level() from
 * nd-level's header but implements nothing from nd-attr's, so it does not
 * include nd/attr.h at all -- there is no XY_DECL to collide with. No header
 * of its own: nobody includes class.
 *
 * Order matters below: XY_IMPL emits a definition, so a caller has to come
 * after its callee.
 */

#include <ttypt/xy-mod.h>

#include <nd/xy.h>

#include <stdlib.h>
#include <string.h>

#include <nd/level.h>

typedef struct {
	unsigned class;
} classist_t;

typedef struct {
	char name[32];
	unsigned life_dice;
	unsigned lvl;
} class_t;

static unsigned classist_hd, class_hd, class_max = 0;

/* Co-implementor of nd-attr's hp_max chain: base value + class life dice
 * scaled by level. nd_last() gives us whatever ran before us in this
 * dispatch. */
XY_IMPL(long, hp_max, unsigned, ref)
{
	classist_t classist;
	class_t class;
	long last;

	nd_get(classist_hd, &classist, &ref);
	nd_get(class_hd, &class, &classist.class);

	nd_last(&last);

	register unsigned d = class.life_dice;
	register unsigned l = level(ref);
	return last + d + (l - 1) * (d / 2 + 1);
}

XY_IMPL(int, on_add, unsigned, ref, unsigned, type, uint64_t, v)
{
	classist_t classist;

	(void) v;
	if (type != TYPE_ENTITY)
		return 1;

	classist.class = random() % (class_max + 1);
	nd_put(classist_hd, &ref, &classist);
	return 0;
}

XY_IMPL(int, on_status, unsigned, player_ref)
{
	classist_t classist;
	class_t class;

	nd_get(classist_hd, &classist, &player_ref);
	nd_get(class_hd, &class, &classist.class);

	nd_printf(player_ref, "Class\t%8s, dlif %3u\n", class.name, class.life_dice);
	return 0;
}

static inline unsigned class_add(char *name, unsigned life_dice) {
	class_t class = { .life_dice = life_dice, };
	strlcpy(class.name, name, sizeof(class.name));
	return class_max = (unsigned)nd_put(class_hd, NULL, &class);
}

XY_MODULE_API void
xy_install(void)
{
	/* Order matches the original mod_install: opens first, then the four
	 * classes. */
	nd_len_reg("class", sizeof(class_t));
	nd_len_reg("classist", sizeof(classist_t));
	class_hd = (unsigned)nd_open("class", "u", "class", ND_AINDEX);
	classist_hd = (unsigned)nd_open("classist", "u", "classist", 0);

	class_add("fighter", 10);
	class_add("sorcerer", 6);
	class_add("cleric", 8);
	class_add("rogue", 8);
}