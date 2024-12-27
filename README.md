# `narc`

An implementation of the Nitro Archive virtual file system used by the Nintendo
DS.

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

## Usage

## API

```c
#include <stddef.h>
#include <stdint.h>

struct narc;

/*
 * Allocate memory for a new NARC.
 */
struct narc *narc_alloc(void);

/*
 * Free allocated NARC memory.
 */
void narc_free(struct narc *narc);

/*
 * Load a NARC from an existing file on disk at the given path.
 */
int narc_load(struct narc *narc, const char *file_path);

/*
 * Write a NARC to a file on disk at the given path.
 */
int narc_write(struct narc *narc, const char *file_path);

/*
 * Add a file image to the end of a NARC. If successful, then `out_file_id` will
 * be set to the file ID of the persisted image.
 */
int narc_add(struct narc *narc, unsigned char file_image[], uint16_t *out_file_id);

/*
 * Read a file image with the given ID from a NARC. If such a file image exists,
 * then `out_file_image` will be set to the in-memory address of the file image
 * within the NARC and `out_file_size` will be set to the byte-size of that
 * image.
 */
int narc_read(struct narc *narc, uint16_t file_id, unsigned char *out_file_image[], size_t *out_file_size);

/*
 * Compute the SHA-1 hash of this NARC.
 *
 * The resulting hash will always be 20 bytes in length.
 */
uint8_t *narc_hash(struct narc *narc);

/*
 * Compute the SHA-1 hash of a file image with the given ID within a NARC. If
 * such a file exists, then `out_file_hash` will be set to the resulting 20-byte
 * hash.
 */
int narc_hash_file(struct narc *narc, uint16_t file_id, uint8_t *out_file_hash[]);

/*
 * Report the number of files contained within a NARC.
 */
uint16_t narc_num_files(struct narc *narc);

/*
 * Print a report on internal information about a NARC. Useful for debugging.
 */
void narc_printf(struct narc *narc);
```

## Acknowledgements

- Martin Korth's [`gbatek` documentation][gbatek], which is an excellent read
  on all matters related to the GameBoy Advance and Nintendo DS. In particular,
  his section on [DS cartridge file systems][gbatek-ds-filesys] was instrumental
  in building this implementation. The relevant documentation has been mirrored
  here in `docs`, for convenience and preservation.

## Contributing

## License

`narc` is free software licensed under the Apache License, version 2.0. For
further details, refer to [the included license text](./LICENSE).

[gbatek]: http://problemkaputt.de/gbatek-contents.htm
[gbatek-ds-filesys]: http://problemkaputt.de/gbatek-ds-cartridge-nitrorom-and-nitroarc-file-systems.htm
