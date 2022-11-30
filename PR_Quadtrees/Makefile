# define C compiler & flags
CC = gcc
CFLAGS = -Wall -g
# define libraries to be linked (for example -lm)
LIB = 

# define sets of header source files and object files
SRC = dict3.c quadtree.c data.c
# OBJ is the same as SRC, just replace .c with .h
OBJ = $(SRC:.c=.o)
 
# exacutable name
# the first target:
dict3: $(OBJ) 
	$(CC) $(CFLAGS) -o dict3 $(OBJ) $(LIB)

# the second target:
dict4: $(OBJ) 
	$(CC) $(CFLAGS) -o dict3 $(OBJ) $(LIB)


dict3.o: dict3.c data.h quadtree.h

quadtree.o: quadtree.c quadtree.h data.h

data.o: data.c data.h

clean:
	rm -f $(OBJ) dict3 dict4


