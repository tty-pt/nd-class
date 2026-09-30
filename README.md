# axil-nd-class

`nd-class` for [axil-nd](../axil-nd), ported from SIC to libxylem.

Owns character classes: the `class` table (auto-indexed), the per-entity
`classist` assignment, and the life-dice bonus. Its one stat role is
co-implementing nd-attr's `hp_max` chain — base value plus class life dice
scaled by level — reading the predecessor with `nd_last()`.

## Install

```sh
make install
```

Installs one file:

```
lib/libnd-class.so
```

There is deliberately no `lib/nd-class.so` symlink (see `axil-nd-wts` for
why: `mods.load` names the installed filename, and the OpenBSD packing list
never lists a symlink).

Also packaged for deb, apk, rpm, brew and openbsd from a `v*` tag.

It installs no header because it exports no API — every symbol it defines is
discovered by the engine, not called by another module.

## Build from source

```sh
make
```

Needs [libxylem](https://github.com/tty-pt/libxylem) and the engine's game
API, `<nd/xy.h>`, plus `<nd/level.h>` (it scales by level) and `<nd/attr.h>`
(the chain it co-implements) — from checkouts beside this repo or from
installed packages:

```sh
git clone https://github.com/tty-pt/nd-class && cd nd-class
git clone https://github.com/tty-pt/axil-nd ../axil-nd
git clone https://github.com/tty-pt/nd-level ../axil-nd-level
git clone https://github.com/tty-pt/nd-attr ../axil-nd-attr
make
```

Both the checkout `-I` flags and the installed-package paths are on the
command line at once (see `Makefile`), and a missing `-I` is ignored, so the
same command works either way. CI names the deps explicitly
(`axil-nd,libxylem,nd-level,nd-attr`).

## What it does

* `xy_install()` registers the `class` table (auto-indexed) and the
  `classist` table (one row per entity).
* `hp_max` adds the class life dice on top of the chain value;
  `on_add` assigns the class; `on_status` reports it.

## Testing

There is no `test.sh` here. Behaviour is asserted by the engine's own suite:

```sh
cd ../axil-nd
make && ./test.sh
```

## Notes from the port

* `SIC_DEF` → `XY_IMPL`, `mod_install` → `xy_install`, `call_f(...)` →
  `f(...)`.
* The link line is libxylem alone. `NEEDED` is `libxylem.so` and `libc.so.6`.

## License

BSD 2-Clause, carried over from `tty-pt/nd-class`. See `LICENSE`.
