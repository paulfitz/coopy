CC=gcc
AR=ar

VERSION=3.0.0
DESTDIR=
LIBDIR=/usr/lib
INCDIR=/usr/include
MANDIR=/usr/share/man/man3

.PHONY: all install install_headers install_man install_shared install_static clean test

all: libcsv.so libcsv.a

install: install_headers install_shared_lib install_static_lib install_man

install_shared: install_headers install_shared_lib
install_static: install_headers install_static_lib

install_man: csv.3.gz
	cp -f $^ $(DESTDIR)$(MANDIR)/

install_headers: csv.h
	mkdir -p $(DESTDIR)$(INCDIR)/libcsv/
	cp -f $^ $(DESTDIR)$(INCDIR)/libcsv/

install_shared_lib: libcsv.so
	cp -f $< $(DESTDIR)$(LIBDIR)/$<.$(VERSION)
	ln -sf $<.$(VERSION) $(DESTDIR)$(LIBDIR)/$<.3
	ln -sf $<.3 $(DESTDIR)$(LIBDIR)/$<

install_static_lib: libcsv.a
	cp -f $< $(DESTDIR)$(LIBDIR)/$<

libcsv.o: libcsv.c csv.h
	$(CC) $< -c -o $@

libcsv.so: libcsv.o
	$(CC) -shared $< -o $@

libcsv.a: libcsv.o
	$(AR) -rc $@ $^
	$(AR) -s $@

clean:
	rm libcsv.o libcsv.so libcsv.a

test: libcsv.o
	$(CC) test_csv.c $< -o $@
	./test
	rm ./test

