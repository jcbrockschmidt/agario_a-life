# project name
NAME = agarioALife

# build environment
CC=g++

# source files
SOURCES := $(wildcard src/*.cpp)

# object files
OBJS := $(SOURCES)
#OBJS := $(patsubst %.cpp,%.o,$(SOURCES))

# Build flags
INCLUDES    := -Iinclude
BASEFLAGS   := -O2
WARNFLAGS   := -Wall -Wextra -Wshadow -Wcast-align -Wwrite-strings -Winline
WARNFLAGS   += -Wno-attributes -Wno-deprecated-declarations
WARNFLAGS   += -Wno-div-by-zero -Wno-endif-labels -Wfloat-equal
WARNFLAGS   += -Wformat=2 -Wno-format-extra-args -Winit-self
WARNFLAGS   += -Winvalid-pch -Wmissing-format-attribute
WARNFLAGS   += -Wmissing-include-dirs -Wno-multichar -Wshadow
WARNFLAGS   += -Wno-sign-compare -Wswitch -Wsystem-headers
WARNFLAGS   += -Wno-pragmas -Wno-unused-but-set-parameter
WARNFLAGS   += -Wno-unused-but-set-variable -Wno-unused-result
WARNFLAGS   += -Wwrite-strings -Wdisabled-optimization -Wpointer-arith
WARNFLAGS   += -Werror
CFLAGS      := $(INCLUDES) $(BASEFLAGS)
CFLAGS      += -std=c++11
LDFLAGS     := -lSDL2

# build rules
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)

warn: CFLAGS += $(WARNFLAGS)
warn: $(NAME)

debug: CFLAGS += $(WARNFLAGS) -g
debug: $(NAME)

clean:
	$(RM) -f $(NAME)
