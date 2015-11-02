# Makefile for the Dev Kit 2.1 daemon
#
CC = g++
SEABREEZEDIR = /home/pi/dev-kit/SeaBreeze

LIBHEADERS = Connection.h   IResponseHandler.h  RequestHandler.h  Spectrometer.h \
	ActiveObject.h           Common.h         Version.h \
	BaseConnectionHandler.h  RequestHandlerConfiguration.h  FileManager.h  Sequence.h

DAEMONHEADERS = OceanHandler.h OceanHandlerConfiguration.h Daemon.h

LIBSOURCE = Connection.cpp    RequestHandler.cpp \
	ActiveObject.cpp           Sequence.cpp \
	BaseConnectionHandler.cpp  FileManager.cpp   Spectrometer.cpp \
	Common.cpp                 RequestHandlerConfiguration.cpp

DAEMONSOURCE = main.cpp OceanHandler.cpp OceanHandlerConfiguration.cpp Daemon.cpp

LIBOBJECTS = Connection.o    RequestHandler.o \
	ActiveObject.o           Sequence.o \
	BaseConnectionHandler.o  FileManager.o   Spectrometer.o \
	Common.o                 RequestHandlerConfiguration.o

DAEMONOBJECTS = main.o OceanHandler.o OceanHandlerConfiguration.o Daemon.o

INCLUDE = -I$(SEABREEZEDIR)/include
LIBS = -lboost_thread -lboost_system -lboost_filesystem -lboost_chrono -lseabreeze -lusb -lpthread 
CFLAGS = -std=c++0x

reqhandler : $(LIBOBJECTS)
	$(CC) $(INCLUDE) $(LIBDIR) $(LIBS) $(CFLAGS) $(LIBOBJECTS) -O -shared -o libreqhandler.2.1.so

ocean-daemon : $(DAEMONOBJECTS) reqhandler
	$(CC) $(INCLUDE) $(LIBDIR) $(LIBS) $(CFLAGS) $(DAEMONOBJECTS) -L./ -lreqhandler -O -o ocean-daemon.2.1

BaseConnectionHandler.o : BaseConnectionHandler.cpp BaseConnectionHandler.h
	$(CC) $(INCLUDE) $(CFLAGS) -O -c BaseConnectionHandler.cpp

RequestHandlerConfiguration.o : RequestHandlerConfiguration.cpp RequestHandlerConfiguration.h
	$(CC) $(INCLUDE) $(CFLAGS) -O -c RequestHandlerConfiguration.cpp

OceanHandlerConfiguration.o : OceanHandlerConfiguration.cpp OceanHandlerConfiguration.h
	$(CC) $(INCLUDE) $(CFLAGS) -O -c OceanHandlerConfiguration.cpp

ActiveObject.o : ActiveObject.cpp ActiveObject.h
	$(CC) $(INCLUDE) $(CFLAGS) -O -c ActiveObject.cpp

RequestHandler.o : RequestHandler.cpp RequestHandler.h
	$(CC) $(INCLUDE) $(CFLAGS) -O -c RequestHandler.cpp

Daemon.o : Daemon.cpp Daemon.h
	$(CC) $(INCLUDE) $(CFLAGS) -O -c Daemon.cpp

OceanHandler.o : OceanHandler.cpp OceanHandler.h
	$(CC) $(INCLUDE) $(CFLAGS) -O -c OceanHandler.cpp

Common.o : Common.cpp Common.h
	$(CC) $(INCLUDE) $(CFLAGS) -O -c Common.cpp

Spectrometer.o : Spectrometer.cpp Spectrometer.h
	$(CC) $(INCLUDE) $(CFLAGS) -O -c Spectrometer.cpp

Sequence.o : Sequence.cpp Sequence.h
	$(CC) $(INCLUDE) $(CFLAGS) -O -c Sequence.cpp

Connection.o : Connection.cpp Connection.h
	$(CC) $(INCLUDE) $(CFLAGS) -O -c Connection.cpp

FileManager.o : FileManager.cpp FileManager.h
	$(CC) $(INCLUDE) $(CFLAGS) -O -c FileManager.cpp

main.o : main.cpp
	$(CC) $(INCLUDE) $(CFLAGS) -O -c main.cpp
