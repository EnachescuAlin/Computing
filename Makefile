CFLAGS = -std=c++11 -Wall
INCLUDE = -I./include

all: main

main:
	g++ $(CFLAGS) -o main src/main.cpp src/token.cpp src/lexer.cpp src/calculation.cpp $(INCLUDE)

clean:
	rm main
