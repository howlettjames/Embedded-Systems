#Archivo Makefile

PROYECTO=main_procesos
DESTDIR=/usr/local/bin
CFLAGS=-Wall -pthread
LFLAGS=-lm -lpthread
CC=gcc

all: $(PROYECTO)

%.o: %.c
	$(CC) -c $< $(CFLAGS)

$(PROYECTO): main_procesos.o arreglos.o procesamiento.o
	$(CC) $^ -o $@ $(LFLAGS)

.PHONY: clean

install: all
	if [ ! -d $(DESTDIR) ]; then \
		sudo mkdir $(DESTDIR); \
	fi; \
	sudo cp $(PROYECTO) $(DESTDIR)

uninstall:
	sudo rm $(DESTDIR)/$(PROYECTO)

clean:
	rm -f *.o $(PROYECTO)
