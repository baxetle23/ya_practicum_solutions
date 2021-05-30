CC=g++
CFLAGS=-c -Wall -Wextra -Werror -std=c++17 -ltbb
LDFLAGS= -ltbb
SOURCES=document.cpp main.cpp process_queries.cpp  read_input_functions.cpp\
		remove_duplicates.cpp request_queue.cpp search_server.cpp string_processing.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC)  $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o