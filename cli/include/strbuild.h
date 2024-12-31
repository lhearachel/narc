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
#ifndef NARC_STRBUILD_H
#define NARC_STRBUILD_H

#include <stddef.h>

struct strbuild {
    char *s;
    size_t len;
    size_t capacity;
};

struct strbuild *strbuild_new(size_t capacity);
void strbuild_del(struct strbuild *strbuild);
int strbuild_cat(struct strbuild *strbuild, const char *s);
int strbuild_sprintf(struct strbuild *strbuild, const char *fmt, ...);

#endif // NARC_STRBUILD_H
