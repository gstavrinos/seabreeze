SEABREEZE = ..

APP = oceanfx_speed_test
OBJS = $(addsuffix .o,$(APP))

all: $(APP)

include $(SEABREEZE)/common.mk

$(APP) : $(OBJS) 
	@echo linking $@
	$(CC) -o $@ $@.o  -lseabreeze $(LFLAGS_APP)
