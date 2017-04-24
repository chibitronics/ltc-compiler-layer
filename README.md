Love-to-Code User Library (and Server Image)
=======================================================

This project provides a runtime library for use with the Love-to-Code and
the ltc-os that it ships with.

It also provides a Makefile, and several example projects.

Finally, it includes a Dockerfile image that sets up an Apache server that
runs a Codebender-compatible API capable of servicing requests.

Directory Structure
-------------------

The directory structure of this project separates the core support files out
from the example projects and the Docker image source files.

    support/   # Source files and support libraries for Arduino compatibility
    projects/  # A selection of example projects
    builder/   # Required files for the Docker image

Projects
========

The Projects/ directory contains many example projects that can provide a good
starting point for understanding how LtC works.  Oftentimes, a new example
project will be created to test a new OS-level feature.

Projects are designed to be easy to compile and quick to setup.

Building an Example Project
---------------------------

To build a project, set TRGT to your compiler's prefix, enter the directory,
and type "make".

For exmaple, if your compiler is arm-none-eabi-gcc, run:

    cd projects/blink
    export TRGT=arm-none-eabi-
    make

Alternatively, you can specify TRGT as part of the "make" command:

    cd projects/blink
    make TRGT=arm-none-eabi-


Creating a New Example Project
------------------------------

To create a new project, copy an existing project into a new directory.  A
good base project is "minimal/".

There are no other setup instructions.  Just type "make".  All .cpp, .c, and .S
(assembly) files will be automatically included.

In case you need to recreate / move some files, you might want to run "make clean".

On Windows, "make clean" may not work, so simply remove the .obj/ directory.


Programming the Output File
---------------------------

To program the output, either use gdb on the resulting .elf file, or use a
modulator such as the Rust-based https://crates.io/crates/ltc-modulate


Builder Image
=============

This directory also contains support files to build a Docker image capable of
acting as a build server.

Building the Docker Image
-------------------------

To build the Docker image, simply use "docker build":

    docker build -t ltc-compiler .

The Dockerfile is set up to be multiarch-compatible, however there is no Makefile
wrapped around it to do actual multiarch builds.  Additionally, you need a special
multiarch-enabled builder, or you need to run it on multiple platforms.

The Dockerfile contains annotations for armhf, in the form of comments.  A Makefile
defined at https://eyskens.me/multiarch-docker-images/ (stored in Github at
https://github.com/multiarch/dockerfile/tree/master/method-g) provides a shell
script/Makefile that can be used to build on various architectures.

Deploying the Docker Image
--------------------------

The image exposes port 80.  If you want SSL, you will need to set up a reverse proxy.

If you need to set CORS headers, you can do so with an environment variable.

An example invocation to start the server listening on port 3457 might be:

    docker run --rm --name ltc-compile-node -it -p 3457:80 -e 'CORS_DOMAINS=*' ltc-compiler