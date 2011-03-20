# ErlXSL Sablotron Provider

This project implements an *XSLT Engine Provider* for the [ErlXSL](http://github.com/hyperthunk/erlxsl) XSLT driver.

## Target System Requirements

[ErlXSL](http://github.com/hyperthunk/erlxsl) has (currently) been tested on OS-X, Debian Linux and several other debian-based distros (including Ubuntu). For a complete list of supported Operating Systems, see the [ErlXSL Wiki](http://github.com/hyperthunk/erlxsl/wiki).

You will ened a version of Sablotron installed and `libsablot` must be available on the appropriate path for dynamic linking to occur (e.g. `LD_LIBRARY_PATH` on linux, `DYLIB_LIBRARAY_PATH` on OS X, etc).

## Build System Requirements

You will need a C99 compatible toolchain installed in order to build from sources.

A version of Sablotron is being mirrored [here](http://github.com/hyperthunk/Sablotron), just in case you have difficulty locating the official download sites (as I did). It is also available for download from sourceforce.

If you have already installed [erlxsl](http://github.com/hyperthunk/erlxsl), you should set the `ERLXSL_HOME` environment variable to the root directory in which it is located. Otherwise, you will need to obtain a copy of the [erlxsl API header file](https://github.com/hyperthunk/erlxsl/blob/master/c_src/erlxsl.h). Although it is *not* necessary to have the Erlang Linked-In driver installed on your system in order to compile this project, you will need to set `ERLXSL_HOME` to the directory in which the header is to be found. In this latter case, you should export an environment variable `ERLXSL_API_FILE` containing the path to the header file.

## Building From Source

After exporting your `ERLXSL_HOME` or `ERLXSL_API_FILE` environment variable, you can build the project using GNU make (or an equivalent tool that understands GNU makefiles). The default *rule* will build (i.e. compile) the sources. In order to run the tests you will need to run `make test` instead.

