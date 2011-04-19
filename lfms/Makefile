CFLAGS=-Wall -O2 -L.
CFLAGS_LIBXML=`xml2-config --cflags`
LDFLAGS_LIBXML=`xml2-config --libs`
LDFLAGS=${LDFLAGS_LIBXML} -lcrypto

all: lfms

lfms: liblfms main.o LfmsConfig.o LfmsSession.o LfmsWsApi.o LfmsTrack.o Lfms.o
	g++ ${CFLAGS} -o lfms \
	main.o LfmsConfig.o LfmsSession.o LfmsWsApi.o LfmsTrack.o Lfms.o \
	Log.o HttpClient.o XmlParser.o helpers.o \
	${LDFLAGS}

liblfms: helpers.cpp Log.cpp HttpClient.cpp XmlParser.cpp
	g++ ${CFLAGS} ${CFLAGS_LIBXML} -c \
	helpers.cpp Log.cpp HttpClient.cpp XmlParser.cpp

main.o LfmsConfig.o LfmsSession.o LfmsWsApi.o LfmsTrack.o Lfms.o: main.cpp LfmsConfig.cpp LfmsSession.cpp LfmsWsApi.cpp LfmsTrack.cpp Lfms.cpp
	g++ ${CFLAGS} ${CFLAGS_LIBXML} -c \
	main.cpp LfmsConfig.cpp LfmsSession.cpp LfmsWsApi.cpp LfmsTrack.cpp Lfms.cpp

clean:
	rm -f helpers.o Log.o HttpClient.o XmlParser.o main.o LfmsConfig.o LfmsSession.o LfmsWsApi.o LfmsTrack.o Lfms.o lfms
