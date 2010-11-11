all: lfms

lfms: main.o LfmsConfig.o helpers.o
	g++ -o lfms main.o LfmsConfig.o helpers.o

main.o: main.cpp
	g++ -c main.cpp

helpers.o: helpers.cpp
	g++ -c helpers.cpp

LfmsConfig.o: LfmsConfig.cpp
	g++ -c LfmsConfig.cpp

clean:
	rm main.o helpers.o LfmsConfig.o lfms
