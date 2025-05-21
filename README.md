# `narc`

An implementation of the Nitro Archive (NARC) virtual file system used by the
Nintendo DS.

This repository provides two code modules:

1. A library implementation of the NARC file specification. This module can be
   compiled as a shared library and linked against other projects which require
   interactivity with NARCs.
2. A CLI, which builds upon the included library to provide a simple program
   for creating NARCs, extracting files from existing NARCs, and viewing
   metadata about existing NARCs.

## Table of Contents

<!--toc:start-->
- [Install](#install)
- [Usage](#usage)
- [API](#api)
- [Acknowledgements](#acknowledgements)
- [Contributing](#contributing)
- [License](#license)
<!--toc:end-->

## Install

### Install from Source

A `Makefile` is provided for convenience, which will build both a shareable
library `libnarc` and a static executable. After acquiring a copy of the source
-- via `git` or a `tar` archive -- `cd` into the source folder and run:

```bash
make install
```

By default:

1. `libnarc.so` will be installed to `~/.local/lib`
2. `narc` will be installed to `~/.local/bin`

If you wish to change the parent directory of these destinations, specify that
parent via `DESTDIR` ahead of the `make` command, e.g.:

```bash
# This will install `libnarc.so` to `/usr/local/lib` and `narc` to
# `/usr/local/bin`.
DESTDIR=/usr/local sudo make install
```

### Integrate via Meson

A `meson.build` file is also provided for integrating `narc` as a subproject. To
add `narc` to your project, create the file `subprojects/narc.wrap` with the
following content:

```ini
[wrap-git]
url = https://github.com/lhearachel/narc.git
; Replace <main> here with a release tag or commit hash, if desired.
revision = main
depth = 1

[provide]
program_names = narc
dependency_names = libnarc
```

## CLI Usage

`narc` ships with rich documentation via its help-text:

```text
narc - create, extract, and explore Nitro Archive virtual filesystems

Usage: narc [-h | --help] [-v | --version] [command] [arguments]

Commands:
  c, create    Create a NARC from a folder of physical files
  x, extract   Extract virtual files from the NARC to a folder
  y, yank      Yank individual files from the NARC to disk
  i, info      Print metadata for a NARC
  h, help      Print help-text for a particular command
```

For brevity, individual commands will not be explained in detail here; refer to
the help-text for individual commands as needed after installation.

## Library API

The meat of `narc` is in the `libnarc` API. After installation, a calling
program should have access to the following files in their library include path:

```c
#include <narc/api/check.h>
#include <narc/api/dump.h>
#include <narc/api/error.h>
#include <narc/api/files.h>
#include <narc/api/load.h>
#include <narc/api/pack.h>
```

If desired, the whole library can also be included with a single include
statement:

```c
#include <narc/narc.h>
```

## Acknowledgements

- Martin Korth's [`gbatek` documentation][gbatek], which is an excellent read
  on all matters related to the GameBoy Advance and Nintendo DS. In particular,
  his section on [DS cartridge file systems][gbatek-ds-filesys] was instrumental
  in building this implementation. The relevant documentation has been mirrored
  here in `docs`, for convenience and preservation.

## License

`narc` is free software licensed under the Apache License, version 2.0. For
further details, refer to [the included license text](./LICENSE).

[gbatek]: http://problemkaputt.de/gbatek-contents.htm
[gbatek-ds-filesys]: http://problemkaputt.de/gbatek-ds-cartridge-nitrorom-and-nitroarc-file-systems.htm
