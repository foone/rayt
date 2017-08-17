CC=g++
CFLAGS=`sdl-config --cflags` `pkg-config --cflags tinyxml`
LINKEROPT=`sdl-config --libs` -lGL -lGLU `pkg-config --libs tinyxml`
OBJS= main.o video.o input.o player.o map.o
OUTPUT=rayt 

all: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $(OUTPUT) $(LINKEROPT)
	
.cpp.o: 
	$(CC) $(CFLAGS) -c $<
clean:
	rm $(OUTPUT) $(OBJS);
 
