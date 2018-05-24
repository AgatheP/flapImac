ifdef COMSPEC
 MV ?= move
 RM ?= del
else
 MV ?= mv -f
 RM ?= rm -f
endif

CC=gcc
CFLAGS= -W -Wall
LDFLAGS= -lSDL -lGLU -lGL -lm -lSDL_image
EXEC=main
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

all: $(EXEC)

main: $(OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS)

main.o: structures.h draw.h parameters.h level.h

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@$(RM) *.o

mrproper: clean
	@$(RM) $(EXEC)
