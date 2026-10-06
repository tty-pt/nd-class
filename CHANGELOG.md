## 1.0.2

- **The sibling `-I` lines are gone.** `CFLAGS += -I$(shell cd .. && pwd)/…`
  pointed at the axil-nd/nd sibling checkouts and only existed for a dev
  build: in CI those directories do not exist and every header comes from the
  installed packages named in `.github/workflows/ci.yml`. The build now
  resolves `<nd/…>` the way a packager sees it.
- **macOS: link with `-undefined dynamic_lookup`.** macOS `ld` rejects
  undefined symbols in a shared library, but `WARN` needs `qsyslog` — an
  engine-provided function pointer resolved at `dlopen` time (Linux allows
  this by default). `-undefined dynamic_lookup` is the Darwin equivalent, set
  as `LDFLAGS-libnd-class-Darwin` so no other platform is affected.

## [1.0.0]

- **nd-class is now an installable library rather than a build artifact of
  the engine.** It builds and installs exactly one file,
  `lib/libnd-class.so`, following the same layout as `axil-tty` and
  `axil-auth`, and the same layout `nd-core` was converted to first.
  Previously `make` produced a `class.so` named by the engine's `mods.load`
  and installed nothing. There is no `lib/nd-class.so` symlink: `mods.load`
  names this module `libnd-class`, the installed filename, and
  `module_load_path()` only appends `.so`. It installs no header because it
  exports no API — every symbol it defines is discovered by the engine, not
  called by another module.

- **The link line is libxylem alone.** `LDLIBS := -lxylem`; the engine is not
  linked. `NEEDED` is `libxylem.so` and `libc.so.6`.

- **The class tables store full structs.** The auto-indexed `class` table
  and the `classist` table are registered with `nd_len_reg`, so
  `hd_mod_open` opens them with the registered value types. `hp_max`
  co-implements nd-attr's chain (base value + class life dice scaled by
  level, read through `nd_last()`).

- **Dropped the `nd-mod.mk` dependency.** `nd-mod.mk` has now been deleted.
