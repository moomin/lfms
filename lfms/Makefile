CFLAGS=-Wall -O2
CFLAGS_LIBXML=`xml2-config --cflags`

all: lfms

lfms: main.o helpers.o Log.o XmlParser.o HttpClient.o LfmsConfig.o LfmsSession.o LfmsWsApi.o LfmsTrack.o Lfms.o
	g++ ${CFLAGS} -o lfms main.o helpers.o Log.o XmlParser.o HttpClient.o LfmsConfig.o LfmsSession.o LfmsWsApi.o LfmsTrack.o Lfms.o -lcrypto `xml2-config --libs`

main.o: main.cpp
	g++ ${CFLAGS} ${CFLAGS_LIBXML} -c main.cpp

helpers.o: helpers.cpp
	g++ ${CFLAGS} -c helpers.cpp

Log.o: Log.cpp
	g++ ${CFLAGS} -c Log.cpp

XmlParser.o: XmlParser.cpp
	g++ ${CFLAGS} ${CFLAGS_LIBXML} -c XmlParser.cpp

HttpClient.o: HttpClient.cpp
	g++ ${CFLAGS} -c HttpClient.cpp

LfmsConfig.o: LfmsConfig.cpp
	g++ ${CFLAGS} -c LfmsConfig.cpp

LfmsSession.o: LfmsSession.cpp
	g++ ${CFLAGS} -c LfmsSession.cpp

LfmsWsApi.o: LfmsWsApi.cpp
	g++ ${CFLAGS} ${CFLAGS_LIBXML} -c LfmsWsApi.cpp

LfmsTrack.o: LfmsTrack.cpp
	g++ ${CFLAGS} -c LfmsTrack.cpp

Lfms.o: Lfms.cpp
	g++ ${CFLAGS} ${CFLAGS_LIBXML} -c Lfms.cpp

clean:
	rm -f main.o helpers.o Log.o XmlParser.o HttpClient.o LfmsConfig.o LfmsSession.o LfmsWsApi.o LfmsTrack.o Lfms.o lfms
