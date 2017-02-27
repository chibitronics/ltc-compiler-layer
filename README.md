Love-to-Code Project with Partial Arduino Compatibility
=======================================================

This project provides a runtime library for use with the Love-to-Code and
the ltc-os that it ships with.

In order to keep Esplanade programs small, most Arduino functions are built
into the operating system, and are accessed with "SVC" calls.  This library
provides those calls, as well as the supporting headers to allow it to
function.

The main library is in the "common/" directory.  All other directories are
sample projects.


Building a Project
==================

To build a project, set TRGT to your compiler's prefix, enter the directory,
and type "make".

For exmaple, if your compiler is arm-none-eabi-gcc, run:

    set TRGT=arm-none-eabi-


Creating a New Project
======================

To create a new project, copy an existing project into a new directory.  A
good base project is "minimal/".

There are no other setup instructions.  Just type "make".


Programming the Output
======================

To program the output, either use gdb on the resulting .elf file, or use a
modulator such as the Rust-based https://crates.io/crates/ltc-modulate
