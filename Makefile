CC=gcc

CFLAGS=-c -Wall -g
LDFLAGS=-pthread

SOURCES=main.c SlowCharacterReader.c FastCharacterReader.c vector.c WordCount.c CharTrie.c word_queue.c ParallelCharacterReader.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=wordcount

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(OBJECTS) $(EXECUTABLE)
