TARGET=main

IDIR=include
CC=g++
CFLAGS=-I$(IDIR) -I/usr/local/include/bullet

ODIR=obj
LDIR=lib
SDIR=src

LIBS=-lm -lstdc++ -lGL -lSDL2 -ltinyobjloader -lBulletDynamics -lBulletCollision -lLinearMath

_DEPS = gameloop.h camera.h debug.h event.h object.h shader.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o gameloop.o camera.o debug.o event.o object.o shader.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: main
.PHONY: clean

clean:
	rm -f $(ODIR)/*.o main 
