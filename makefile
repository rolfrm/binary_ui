OPT = -g3 -O0
LIB_SOURCES1 = main.c 
LIB_SOURCES = $(addprefix src/, $(LIB_SOURCES1)) libmicroio/src/microio.c
CC = gcc
TARGET = run
LIB_OBJECTS =$(LIB_SOURCES:.c=.o)
LDFLAGS= -L. $(OPT) # -Wl,--gc-sections -Wl,-s -fdata-sections -ffunction-sections
LIBS= 
ALL= $(TARGET)
CFLAGS = -Isrc/ -Ilibmicroio/include -Iinclude/ -std=gnu11 -c $(OPT) -Werror -Werror=implicit-function-declaration -Wformat=0 -D_GNU_SOURCE -fdiagnostics-color  -Wwrite-strings -msse4.2 -Werror=uninitialized -DUSE_VALGRIND -DDEBUG -Wall

$(TARGET): $(LIB_OBJECTS)
	$(CC) $(LDFLAGS) $(LIB_OBJECTS) $(LIBS) -o $@

all: $(ALL)

.c.o: $(HEADERS) $(LEVEL_CS)
	$(CC) $(CFLAGS) $< -o $@ -MMD -MF $@.depends

src/ttf_font.c: 
	xxd -i /usr/share/fonts/truetype/dejavu/DejaVuSans.ttf > src/ttf_font.c
main.o: ttf_font.c

depend: h-depend
clean:
	rm -f $(LIB_OBJECTS) $(ALL) src/*.o.depends src/*.o src/level*.c src/*.shader.c 
.PHONY: test
test: $(TARGET)
	make -f makefile.compiler
	make -f makefile.test test

-include $(LIB_OBJECTS:.o=.o.depends)


