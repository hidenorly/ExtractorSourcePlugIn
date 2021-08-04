# compiler env.
UNAME := $(shell uname -s)
ifeq ($(UNAME),Linux)
	CXX=ccache clang++
	LDLIBS=-ldl -lasound
	CXXFLAGS=-std=c++2a -MMD -MP -Wall -fPIC
	SHARED_CXXFLAGS= -fPIC -shared
endif
ifeq ($(UNAME),Darwin)
	CXX=ccache clang++
	LDLIBS=-stdlib=libc++
	CXXFLAGS=-std=c++2a -MMD -MP -Wall
#	SHARED_CXXFLAGS= -flat_namespace -dynamiclib
	SHARED_CXXFLAGS=-dynamiclib
endif

LDFLAGS=-pthread

# project config
EXI_DIR ?= ./src
AFW_DIR=../audioframework
INC_DIR=$(AFW_DIR)/include
LIB_DIR=$(AFW_DIR)/lib
LIB_SOURCE_DIR=$(LIB_DIR)/source-plugin
OBJ_DIR=./out

# --- source code config --------------
EXI_SRCS = $(wildcard $(EXI_DIR)/*.cpp)

# --- the object files config --------------
EXI_OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(EXI_SRCS:.cpp=.o)))

# --- Build for extractor source (shared) ------------
UNAME := $(shell uname -s)
ifeq ($(UNAME),Linux)
	EXI_SO_TARGET = $(LIB_SOURCE_DIR)/libsource_extractor.so
	AFW_SO_TARGET = $(LIB_DIR)/libafw.so
endif
ifeq ($(UNAME),Darwin)
	EXI_SO_TARGET = $(LIB_SOURCE_DIR)/libsource_extractor.dylib
	AFW_SO_TARGET = $(LIB_DIR)/libafw.dylib
endif
EXI_DEPS = $(EXI_OBJS:.o=.d)

default: $(EXI_SO_TARGET)
.PHONY: default

$(EXI_SO_TARGET): $(EXI_OBJS)
	@[ -d $(LIB_DIR) ] || mkdir -p $(LIB_DIR)
	@[ -d $(LIB_SOURCE_DIR) ] || mkdir -p $(LIB_SOURCE_DIR)
	$(CXX) $(LDFLAGS) $(SHARED_CXXFLAGS) $(EXI_OBJS) -o $@ $(LDLIBS) $(AFW_SO_TARGET)

$(EXI_OBJS): $(EXI_SRCS)
	@[ -d $(OBJ_DIR) ] || mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -I $(INC_DIR) -c $(EXI_DIR)/$(notdir $(@:.o=.cpp)) -o $@

-include $(EXI_DEPS)

# --- clean up ------------------------
clean:
	rm -f $(EXI_OBJS) $(EXI_SO_TARGET)
