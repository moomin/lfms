CFLAGS=`xml2-config --cflags`

all: lfms

lfms: main.o helpers.o XmlParser.o HttpClient.o LfmsConfig.o LfmsSession.o LfmsWsApi.o Lfms.o
	g++ -o lfms main.o helpers.o XmlParser.o HttpClient.o LfmsConfig.o LfmsSession.o LfmsWsApi.o Lfms.o -lcrypt -lssl `xml2-config --libs`

main.o: main.cpp
	g++ -c main.cpp

helpers.o: helpers.cpp
	g++ -c helpers.cpp

XmlParser.o: XmlParser.cpp
	g++ -c ${CFLAGS} XmlParser.cpp

HttpClient.o: HttpClient.cpp
	g++ -c HttpClient.cpp

LfmsConfig.o: LfmsConfig.cpp
	g++ -c LfmsConfig.cpp

LfmsSession.o: LfmsSession.cpp
	g++ -c LfmsSession.cpp

LfmsWsApi.o: LfmsWsApi.cpp
	g++ -c ${CFLAGS} LfmsWsApi.cpp

Lfms.o: Lfms.cpp
	g++ -c Lfms.cpp

clean:
	rm -f main.o helpers.o XmlParser.o HttpClient.o LfmsConfig.o LfmsSession.o LfmsWsApi.o Lfms.o lfms
