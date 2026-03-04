##############################################
# Makefile for CST320 labs
#
# Author: Anthony Jewell
#

COPTS=-Wall -g -c -O0
OBJS=main.o \
	 langlex.o \
	 parse.o \
	 utils.o

all: calc

clean:
	rm -f $(OBJS)
	rm -f langlex.c
	rm -f calc
	rm -f lang
	rm -f out

.c.o:
	g++ $(COPTS) $? -o $@

.cpp.o:
	g++ $(COPTS) $? -o $@

main.o: main.cpp langlex.c 
	g++ $(COPTS) main.cpp -o main.o

langlex.c: lang.l
	flex -o langlex.c lang.l

langlex.o: langlex.c
	g++ $(COPTS) -Wno-sign-compare $? -o $@

calc: $(OBJS)
	g++ $(OBJS) -o calc

lang: calc
	cp calc lang
