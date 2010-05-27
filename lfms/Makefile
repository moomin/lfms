all: lfms

lfms: main.o
	g++ -o lfms main.o

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm main.o lfms