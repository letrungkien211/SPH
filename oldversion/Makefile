CC = g++ -g -std=c++0x
RM = rm -rf

LIBS = -lglut -lGL -lGLU -lm -lpthread #`pkg-config opencv --libs`
INCLUDES = #`pkg-config opencv --cflags`

TARGET = main
OBJ = macro.o particle.o system.o grids.o
all: $(TARGET)

.cpp.o:
	$(CC) -c $< $(INCLUDES)
$(TARGET): %: %.o $(OBJ)
	$(CC) -o $@ $^ $(LIBS)
clean:
	$(RM) *.o $(TARGET) *~ *#*