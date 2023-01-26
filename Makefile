CC = gcc
CFLAGS = -Wall -Werror
SOURCES = wish.c
HEADERS = wish.h
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = wish

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(HEADERS)
	$(CC)  $(OBJECTS) -o $@

%.o: %.c $(HEADERS)
	$(CC)  -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)


