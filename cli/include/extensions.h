#ifndef NARC_EXTENSIONS_H
#define NARC_EXTENSIONS_H

// NOTE: This performs a string allocation! The calling client is responsible
// for freeing the result when they are finished with it!
char *guess_extension(const char *data);

#endif // NARC_EXTENSIONS_H
