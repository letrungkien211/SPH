CC = g++ -g -std=c++0x
RM = rm -rf

LIBS = -lglut -lGL -lGLU -lm -lpthread -fopenmp

TARGET = main 
OBJ = sph.o macro.o
all: $(TARGET)

.cpp.o:
	$(CC) -c $< 
$(TARGET): %: %.o $(OBJ)
	$(CC) -o $@ $^ $(LIBS) 
clean:
	$(RM) *.o $(TARGET) *~ *#*