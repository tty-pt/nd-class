all := libnd-class

LDLIBS-libnd-class := -lxylem

FOLDER := nd


# macOS ld rejects undefined symbols in shared libs, but WARN needs
# qsyslog: an engine-provided function pointer resolved at dlopen (Linux
# allows this by default). dynamic_lookup is the Darwin equivalent.
LDFLAGS-libnd-class-Darwin += -undefined dynamic_lookup
-include ./../mk/include.mk
