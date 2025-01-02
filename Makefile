# Copyright 2024 <lhearachel@proton.me>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
TARGET = narc
DESTDIR ?= ~/.local
UNAME_S = $(shell uname -s)

ifneq (,$(findstring Linux,$(UNAME_S)))
CLITARGET = $(TARGET)
LIBTARGET = lib$(TARGET).so
else
ifneq (,$(findstring Darwin,$(UNAME_S)))
CLITARGET = $(TARGET)
LIBTARGET = lib$(TARGET).dynlib
else
# TODO: Windows support
$(error Windows unsupported)
endif
endif

CFLAGS += -MMD -Wall -Wextra -Wpedantic -std=c99
CFLAGS += -I./lib/include

LIBAPI = check dump error files load pack
LIBINC = $(wildcard lib/include/*.h) $(wildcard lib/include/*/*.h)
LIBSRC = $(foreach api,$(LIBAPI),$(wildcard lib/src/$(api)/*.c))
LIBOBJ = $(LIBSRC:.c=.o)
LIBDEP = $(LIBSRC:.c=.d)

CLIINC = $(wildcard cli/include/*.h)
CLISRC = $(wildcard cli/src/*.c)
CLIOBJ = $(CLISRC:.c=.o)
CLIDEP = $(CLISRC:.c=.d)
CLIVER = cli/include/version.h

ALLSRC = $(CLISRC) $(LIBSRC)
ALLINC = $(CLIINC) $(LIBINC)

VERSION = VERSION

.PHONY: all cli lib debug release clean install version

all: lib cli

cli: $(CLIVER) $(CLITARGET)

lib: $(LIBTARGET)

debug: CFLAGS += -g -O0
debug: clean all

release: CFLAGS += -DNDEBUG -O3
release: clean all

clean:
	$(RM) $(LIBTARGET) $(LIBOBJ) $(LIBDEP) $(CLITARGET) $(CLIOBJ) $(CLIDEP) $(CLIVER)

-include $(LIBDEP)
-include $(CLIDEP)

version:
$(CLIVER): tools/version.sh $(VERSION)
	$< $(VERSION) $@

# Statically link the CLI
$(CLITARGET): CFLAGS += -I./cli/include
$(CLITARGET): $(CLIOBJ) $(LIBOBJ)
	$(CC) $(LDFLAGS) -o $@ $^

$(LIBTARGET): LDFLAGS += -shared
$(LIBTARGET): $(LIBOBJ)
	$(CC) $(LDFLAGS) -o $@ $^

install: lib cli
	mkdir -pm 755 $(DESTDIR)/bin
	mkdir -pm 755 $(DESTDIR)/lib
	mkdir -pm 755 $(DESTDIR)/include
	mkdir -pm 755 $(DESTDIR)/include/narc
	mkdir -pm 755 $(DESTDIR)/include/narc/api
	mkdir -pm 755 $(DESTDIR)/include/narc/defs

	install -m 755 -t $(DESTDIR)/bin $(CLITARGET)
	install -m 644 -t $(DESTDIR)/lib $(LIBTARGET)
	install -m 644 -t $(DESTDIR)/include/narc $(wildcard lib/include/*.h)
	install -m 644 -t $(DESTDIR)/include/narc/api $(wildcard lib/include/api/*.h)
	install -m 644 -t $(DESTDIR)/include/narc/defs $(wildcard lib/include/defs/*.h)

# Compile library sources with position-independent code for shared object link
lib/src/%.o: CFLAGS += -fpic
lib/src/%.o: lib/src/%.c
