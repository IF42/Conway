CC=gcc


CFLAGS += -Wall 
CFLAGS += -Wextra 
CFLAGS += -pedantic
CFLAGS += -Ofast 
CFLAGS += $$(pkg-config --cflags raylib)

LIBS=-lm $$(pkg-config --libs raylib)


CACHE=.cache
OUTPUT=$(CACHE)/release


SRC=src
APP=app

TARGET=conway


ifeq ($(OS),Windows_NT)
    CFLAGS += -mwindows
else
endif



MODULE += main.o
OBJ=$(addprefix $(CACHE)/,$(MODULE))


build: env $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $(OUTPUT)/$(TARGET)


%.o:
	$(CC) $(CFLAGS) -c $< -o $@


-include dep.list


exec: build 
	$(OUTPUT)/$(TARGET)


.PHONY: env dep clean


dep:
	$(CC) -MM $(APP)/*.c $(TEST)/*.c | sed 's|[a-zA-Z0-9_-]*\.o|$(CACHE)/&|' > dep.list


env:
	mkdir -pv $(CACHE)
	mkdir -pv $(OUTPUT)


clean: 
	rm -vrf $(CACHE)


