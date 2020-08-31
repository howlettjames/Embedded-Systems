#Archivo Makefile

PROYECTO=pulso_demonio
DESTDIR=/usr/local/bin
CFLAGS=-Wall
LFLAGS=-lm -lpthread
CC=gcc

all: $(PROYECTO)

%.o: %.c
	$(CC) -c $< $(CFLAGS)

$(PROYECTO): pulso_demonio.o archivos.o procesamiento.o
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
