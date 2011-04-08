CFLAGS=-Wall -O2 -L.
CFLAGS_LIBXML=`xml2-config --cflags`
LDFLAGS_LIBXML=`xml2-config --libs`

all: lfms

lfms: liblfms.dylib main.o LfmsConfig.o LfmsSession.o LfmsWsApi.o LfmsTrack.o Lfms.o
	g++ ${CFLAGS} -o lfms main.o LfmsConfig.o LfmsSession.o LfmsWsApi.o LfmsTrack.o Lfms.o -llfms

liblfms.dylib: helpers.cpp Log.cpp HttpClient.cpp XmlParser.cpp
	g++ -shared ${CFLAGS} ${CFLAGS_LIBXML} ${LDFLAGS_LIBXML} -lcrypto -o liblfms.dylib \
	helpers.cpp Log.cpp HttpClient.cpp XmlParser.cpp

main.o LfmsConfig.o LfmsSession.o LfmsWsApi.o LfmsTrack.o Lfms.o: main.cpp LfmsConfig.cpp LfmsSession.cpp LfmsWsApi.cpp LfmsTrack.cpp Lfms.cpp
	g++ ${CFLAGS} ${CFLAGS_LIBXML} -c \
	main.cpp LfmsConfig.cpp LfmsSession.cpp LfmsWsApi.cpp LfmsTrack.cpp Lfms.cpp

clean:
	rm -f liblfms.dylib main.o LfmsConfig.o LfmsSession.o LfmsWsApi.o LfmsTrack.o Lfms.o lfms
