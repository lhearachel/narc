/*
 * Copyright 2024 <lhearachel@proton.me>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef NARC_STRUTIL_H
#define NARC_STRUTIL_H

#include <stdbool.h>
#include <stddef.h>

bool match_either(const char *s, const char *a, const char *b);
char *basename(const char *path);

// Return the length of the input string `s` if it were to be right-trimmed
// of trailing whitespace.
size_t nrtrim(const char *s);

// NOTE: These routines perform a string allocation! The calling client is
// responsible for freeing the result when they are finished with it!
char *basename_extend(const char *path, const char *ext);
char *strcpy_fext(const char *path, const char *ext);

// Return true if "`parent`/`file`" is a directory, false otherwise.
// If file is not given, then `parent` will be tested without the trailing slash.
bool isdir(const char *parent, const char *file);

// Comparison function for Q sort.
int strcmp_q(const void *s1, const void *s2);

#endif // NARC_STRUTIL_H
