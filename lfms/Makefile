all: lfms

lfms: main.o helpers.o HttpClient.o LfmsConfig.o LfmsSession.o LfmsWsApi.o
	g++ -o lfms main.o helpers.o HttpClient.o LfmsConfig.o LfmsSession.o LfmsWsApi.o -lcrypt -lssl

main.o: main.cpp
	g++ -c main.cpp

helpers.o: helpers.cpp
	g++ -c helpers.cpp

HttpClient.o: HttpClient.cpp
	g++ -c HttpClient.cpp

LfmsConfig.o: LfmsConfig.cpp
	g++ -c LfmsConfig.cpp

LfmsSession.o: LfmsSession.cpp
	g++ -c LfmsSession.cpp

LfmsWsApi.o: LfmsWsApi.cpp
	g++ -c LfmsWsApi.cpp

clean:
	rm -f main.o helpers.o HttpClient.o LfmsConfig.o LfmsSession.o LfmsWsApi.o lfms
