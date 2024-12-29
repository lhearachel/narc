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
CWD_BASE = $(shell basename $(CURDIR))

CLITARGET = $(CWD_BASE)
LIBTARGET = lib$(CWD_BASE).so

CFLAGS += -MMD -Wall -Wextra -Wpedantic -std=c99

# TODO: This will need to be changed when building for installation
CFLAGS += -Ilib/include

LIBAPI = check dump error files load pack
LIBINC = $(wildcard lib/include/*.h) $(wildcard lib/include/*/*.h)
LIBSRC = $(foreach api,$(LIBAPI),$(wildcard lib/src/$(api)/*.c))
LIBOBJ = $(LIBSRC:.c=.o)
LIBDEP = $(LIBSRC:.c=.d)

CLIINC = $(wildcard cli/include/*.h)
CLISRC = $(wildcard cli/src/*.c)
CLIOBJ = $(CLISRC:.c=.o)
CLIDEP = $(CLISRC:.c=.d)

ALLSRC = $(CLISRC) $(LIBSRC)
ALLINC = $(CLIINC) $(LIBINC)

.PHONY: all cli lib debug release clean
.NOTPARALLEL: lib cli

all: lib cli

cli: $(CLITARGET)

lib: $(LIBTARGET)

debug: CFLAGS += -g -O0
debug: clean all

release: CFLAGS += -DNDEBUG -O3
release: clean all

clean:
	$(RM) $(LIBTARGET) $(LIBOBJ) $(LIBDEP) $(CLITARGET) $(CLIOBJ) $(CLIDEP)

-include $(LIBDEP)
-include $(CLIDEP)

# TODO: Replace rpath with something more robust...
$(CLITARGET): LDFLAGS += -Wl,-rpath=$(CURDIR)
$(CLITARGET): LDFLAGS += -L$(CURDIR)
$(CLITARGET): LDFLAGS += -l$(CWD_BASE)
$(CLITARGET): CFLAGS += -Icli/include
$(CLITARGET): $(CLIOBJ)
	$(CC) $(LDFLAGS) -o $@ $^

$(LIBTARGET): LDFLAGS += -shared
$(LIBTARGET): $(LIBOBJ)
	$(CC) $(LDFLAGS) -o $@ $^

# Compile library sources with position-independent code
lib/src/%.o: CFLAGS += -fpic
lib/src/%.o: lib/src/%.c
