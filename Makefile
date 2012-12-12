# CC     = g++
CC     = clang++
# CC     = clang++ -std=c++11 -stdlib=libc++
CFLAGS = -Wall -lsqlite3
LFLAGS =
MFLAGS =

PROGRAMS = main
OBJLIBS  = CSCIx229.a Util.a shape.a guitar.a hand.a Database.a

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
CSCIx229.a:CSCIx229/fatal.o CSCIx229/loadtexbmp.o CSCIx229/print.o CSCIx229/project.o CSCIx229/errcheck.o CSCIx229/object.o
	ar -rcs CSCIx229.a $^

Util.a: Util.o
	ar -rcs Util.a $^

shape.a: shape.o
	ar -rcs shape.a $^

hand.a: hand.o
	ar -rcs hand.a $^

Database.a: Database.o
	ar -rcs Database.a $^


# =============================
SYS:
	@echo $(OSTYPE)

clean:
	@echo cleaning up in .
	rm -f *.o *.a $(PROGRAMS)
