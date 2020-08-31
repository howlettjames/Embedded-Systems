#Archivo Makefile

PROYECTO=muestreo
DESTDIR=/usr/local/bin
CFLAGS=-Wall
LFLAGS=-lm -lpthread
CC=gcc

all: $(PROYECTO)

%.o: %.c
	$(CC) -c $< $(CFLAGS)

#muestreo.o: muestreo.c
#	$(CC) -c muestreo.c $(CFLAGS)

#procesamiento.o: procesamiento.c
#	$(CC) -c procesamiento.c $(CFLAGS)

#archivos.o: archivos.c 
#	$(CC) -c archivos.c $(CFLAGS)

$(PROYECTO): muestreo.o archivos.o procesamiento.o
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
