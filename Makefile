CC = mpic++
CFLAGS = -Wall -O3
LFLAGS = -Wall

SOURCES = alfabet.cpp FileHandler.cpp Lab0Graph.cpp LZWencoder.cpp LZWencoder_U.cpp LZWpair.cpp LZWpair_U.cpp UserInterface.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = program

all: $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)