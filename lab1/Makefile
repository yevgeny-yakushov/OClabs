CC = gcc
CFLAGS = -fsanitize=address 
LIBDIR = ./lib/
SOURCEDIR = ./source/
LIBS = arc

all: 	arc
	$(CC) -o main main.c -I$(SOURCEDIR) -L$(LIBDIR) -l$(LIBS) $(CFLAGS)

arc:	$(SOURCEDIR)arc.o	
	$(CC) -c $(SOURCEDIR)arc.c -o $(SOURCEDIR)arc.o
	$ ar crv $(LIBDIR)libarc.a $(SOURCEDIR)arc.o


