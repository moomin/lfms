all: lfms

lfms: main.o helpers.o LfmsConfig.o LfmsSession.o
	g++ -o lfms main.o LfmsConfig.o LfmsSession.o helpers.o

main.o: main.cpp
	g++ -c main.cpp

helpers.o: helpers.cpp
	g++ -c helpers.cpp

LfmsConfig.o: LfmsConfig.cpp
	g++ -c LfmsConfig.cpp

LfmsSession.o: LfmsSession.cpp
	g++ -c LfmsSession.cpp

clean:
	rm main.o helpers.o LfmsConfig.o LfmsSession.o lfms
