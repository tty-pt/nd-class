#include <stdlib.h>
#include <stdio.h>

#include <nd/nd.h>
#include <nd/type.h>
#include <nd/level.h>
#include <nd/attr.h>

typedef struct {
	unsigned class;
} classist_t;

typedef struct {
	char name[32];
	unsigned life_dice;
	unsigned lvl;
} class_t;

unsigned classist_hd, class_hd, class_max = 0;

long hp_max(unsigned ref) {
	classist_t classist;
	class_t class;
	long last;

	nd_get(classist_hd, &classist, &ref);
	nd_get(class_hd, &class, &classist.class);

	sic_last(&last);

	register unsigned d = class.life_dice;
	register unsigned l = call_level(ref);
	return last + d + (l - 1) * (d / 2 + 1);
}

int on_add(unsigned ref, unsigned type, uint64_t v __attribute__((unused))) {
	classist_t classist;

	if (type != TYPE_ENTITY)
		return 1;

	classist.class = random() % (class_max + 1);
	nd_put(classist_hd, &ref, &classist);
	return 0;
}

int
on_status(unsigned player_ref)
{
	classist_t classist;
	class_t class;

	nd_get(classist_hd, &classist, &player_ref);
	nd_get(class_hd, &class, &classist.class);

	nd_writef(player_ref, "Class\t%8s, dlif %3u\n", class.name, class.life_dice);
	return 0;
}

static inline unsigned class_add(char *name, unsigned life_dice) {
	class_t class = { .life_dice = life_dice, };
	strlcpy(class.name, name, sizeof(class.name));
	return class_max = nd_put(class_hd, NULL, &class);
}

void mod_open(void) {
	nd_len_reg("class", sizeof(class_t));
	nd_len_reg("classist", sizeof(classist_t));
	class_hd = nd_open("class", "u", "class", ND_AINDEX);
	classist_hd = nd_open("classist", "u", "classist", 0);

	class_add("fighter", 10);
	class_add("sorcerer", 6);
	class_add("cleric", 8);
	class_add("rogue", 8);
}

void mod_install(void) {
	mod_open();
}
