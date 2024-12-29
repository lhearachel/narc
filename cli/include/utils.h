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
#ifndef NARC_UTILS_H
#define NARC_UTILS_H

#include <stdbool.h>

bool match_either(const char *s, const char *a, const char *b);
char *basename(const char *path);

// NOTE: This routine performs a string allocation! The calling client is
// responsible for freeing the result when they are finished with it!
char *basename_extend(const char *path, const char *ext);

#endif // NARC_UTILS_H
