Esplanade OS with Partial Arduino Compatibility
===============================================

This project provides an OS and bootloader for use with the Esplanade project.
In order to keep Esplanade programs small, most Arduino functions are built
as libraries that are then accessed using svc instructions.

There are several sub-projects present here:

    os/     Base bootloader.  This sets up handlers, including the SVC_Handler
            to correctly execute Arduino calls.  It also jumps to the program,
            or loads a new program if necessary.
    app/    Application layer, including the boot header and library.
