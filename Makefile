CC = gcc
LD= gcc
CFLAGS = -Wall -std=gnu11
LIB = -llua -lncursesw -lm -lmenu
INC = -Isrc/

CFLAGS_DEBUG = $(CFLAGS) -g
LIB_DEBUG = $(LIB)
LIBDIR_DEBUG = $(LIBDIR)
INC_DEBUG = $(INC)
OBJDIR_DEBUG = obj/Debug
OUT_DEBUG = bin/Debug/thea

CFLAGS_RELEASE = $(CFLAGS) -O2 -march=native
LIB_RELEASE = $(LIB)
LIBDIR_RELEASE = $(LIBDIR)
INC_RELEASE = $(INC)
OBJDIR_RELEASE = obj/Release
OUT_RELEASE = bin/Release/thea

SOURCES = $(wildcard src/*.c)

OBJ_DEBUG = $(patsubst src/%.c, $(OBJDIR_DEBUG)/%.o, $(SOURCES))
OBJ_RELEASE = $(patsubst src/%.c, $(OBJDIR_RELEASE)/%.o, $(SOURCES))

all: debug release

debug: before_debug $(OBJ_DEBUG) out_debug

release: before_release $(OBJ_RELEASE) out_release

before_debug:
	test -d bin/Debug || mkdir -p bin/Debug
	test -d obj/Debug || mkdir -p obj/Debug
	test -d obj/Debug/utils || mkdir -p obj/Debug/utils

before_release:
	test -d bin/Release || mkdir -p bin/Release
	test -d obj/Release || mkdir -p obj/Release
	test -d obj/Release/utils || mkdir -p obj/Release/utils

clean: clean_debug clean_release

clean_debug:
	rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	rm -rf bin/Debug
	rm -rf $(OBJDIR_DEBUG)

clean_release:
	rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	rm -rf bin/Release
	rm -rf $(OBJDIR_RELEASE)

out_debug: $(OBJ_DEBUG)
	$(LD) $(LIBDIR_DEBUG) $(OBJ_DEBUG) $(LIB_DEBUG) -o $(OUT_DEBUG)

out_release: $(OBJ_RELEASE)
	$(LD) $(LIBDIR_RELEASE) $(OBJ_RELEASE) $(LIB_RELEASE) -o $(OUT_RELEASE)

$(OBJDIR_DEBUG)/%.o: src/%.c
	$(CC) $(INC_DEBUG) $(CFLAGS_DEBUG) -c $< -o $@

$(OBJDIR_RELEASE)/%.o: src/%.c
	$(CC) $(INC_RELEASE) $(CFLAGS_RELEASE) -c $< -o $@
