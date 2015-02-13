SEABREEZE = ..

APPS = $(APP) heliospectraSTSdaemon 
OBJS = $(addsuffix .o,$(APPS))
ELOOP = heliospectraSTSdaemon_eventLoop.o
STS= heliospectraSTSdaemon_support.o

all: $(APPS)

include $(SEABREEZE)/common.mk

$(APPS) : $(OBJS) $(ELOOP) $(STS)
	@echo linking $@
	$(CC) -o $@ $@.o $(ELOOP) $(STS) -lseabreeze $(LFLAGS_APP)