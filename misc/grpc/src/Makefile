# Resembla: Word-based Japanese similar sentence search library
# https://github.com/tuem/resembla
#
# Copyright 2017 Takashi Uemura
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

BIN = resembla_server resembla_async_server

all: $(BIN)

CXX := g++
CXXFLAGS := -Wall -Wextra -O3 -std=c++11 `pkg-config --cflags grpc++ grpc` -isystem../include -I../../../src -isystem../../../include -isystem../../../include/json -isystem../../../include/cmdline -isystem../../../include/paramset
CXXLIBS := `pkg-config --libs protobuf grpc++ grpc` -lgrpc++_reflection -lpthread -ldl -lresembla

SUBDIRS = grpc


SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS))
DEPS = $(patsubst %.cpp,%.d,$(SRCS))

RESEMBLA_GRPC_SRCS = $(wildcard grpc/*.cc)
RESEMBLA_GRPC_OBJS = $(patsubst %.cc,%.o,$(RESEMBLA_GRPC_SRCS))


_grpc:
	$(MAKE) -C grpc

resembla_server: resembla_server.o _grpc
	$(CXX) $(CXXLIBS) -o $@ resembla_server.o $(RESEMBLA_GRPC_OBJS)

resembla_async_server: resembla_async_server.o _grpc
	$(CXX) $(CXXLIBS) -o $@ resembla_async_server.o $(RESEMBLA_GRPC_OBJS)


clean: $(SUBDIRS)
	$(MAKE) -C grpc clean
	$(RM) $(OBJS) $(DEPS) $(BIN)


.PHONY: clean all $(SUBDIRS)


.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

%.d: %.cpp
	@set -e; $(CXX) -MM $(CXXFLAGS) $< \
	| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
	[ -s $@ ] || rm -f $@
-include $(DEPS)
