##**************************************
## Makefile
##
## Build routine for lang compiler
##
## Author: Phil Howard 
## phil.howard@oit.edu
##
## Date: Jan. 18, 2015
##

COPTS=-Wall -g -c  -O0 -std=c++11
OBJS=main.o \
	 langlex.o \
	 langparse.o \
	 cSymbolTable.o \
	 cVisitor.o

all: lang

clean:
	rm -f $(OBJS)
	rm -f langlex.c
	rm -f langparse.c
	rm -f langparse.h
	rm -f lang
	rm -f out.xml
	rm -f out2.xml

.cpp.o:
	g++ $(COPTS) $? -o $@

.c.o:
	g++ $(COPTS) $? -o $@

main.o: main.cpp langparse.c langlex.c 
	g++ $(COPTS) main.cpp -o $@

langlex.o: langlex.c
	g++ $(COPTS) -Wno-sign-compare $? -o $@

langlex.c: lang.l langparse.c
	flex -o langlex.c lang.l

langparse.c: lang.y
	bison --defines=langparse.h lang.y -o langparse.c

lang: $(OBJS)
	g++ $(OBJS) -o lang
