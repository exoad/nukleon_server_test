==== Nukleon_Server ====

A reimplementation of Nukleon's backend logic entirely in C.

Currently supporting: Linux, Windows, OSX*

(macOS/OSX is not supported for the GUI portion)

==== Building ====

To build the server and test it, run it with the bundled
commands in `run.sh` or directly compiling with `make -re`

---- Enabling GUI ----

This bundles a GUI for directly seeing what is happening
and allows for a small amount of direction visual modifications.

To build with the GUI, use `USE_UI=1` (i.e. `make USE_UI=1 -re`)
