# CC     = g++
CC     = clang++
CFLAGS = -Wall
LFLAGS =
MFLAGS =

PROGRAMS = main
OBJLIBS  = CSCIx229.a Util.a shape.a guitar.a hand.a

SYSNAME=$(shell uname -s)
ifeq ($(SYSNAME),Darwin)
	LFLAGS += -framework GLUT -framework OpenGL
else
	ifeq ($(SYSNAME),Linux)
		LFLAGS += -lglut -lGLU
	else
		LFLAGS += -lglut32cu -lglu32 -lopengl32
		CLEAN=del *.exe *.o *.a
	endif
endif
# C++11     -std=gnu++0x   -std=c++0x

all: $(PROGRAMS)

#  Generic compile rules
.cpp.o:
	$(CC) -c -O -Wall $<

#  Generic compile and link
%: %.cpp $(OBJLIBS)
	$(CC) $(CFLAGS) -O3 -o $@ $^ $(LFLAGS)

guitar.a: guitar.o
	ar -rcs guitar.a $^

#  Create archive (include glWindowPos here if you need it)
CSCIx229.a:fatal.o loadtexbmp.o print.o project.o errcheck.o object.o
	ar -rcs CSCIx229.a $^

Util.a: Util.o
	ar -rcs Util.a $^

shape.a: shape.o
	ar -rcs shape.a $^

hand.a: hand.o
	ar -rcs hand.a $^


# =============================
SYS:
	@echo $(OSTYPE)

clean:
	@echo cleaning up in .
	rm -f *.o *.a $(PROGRAMS)
