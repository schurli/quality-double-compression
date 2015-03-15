
all:
	cd lib/jxrlib-master/ && $(MAKE)
	cd lib/jpeg-9a/ && ./configure && $(MAKE)
	cd lib/openjpeg-2.1.0/ && cmake . && $(MAKE)
	cd lib/iqa/ && $(MAKE) RELEASE=1

bitrate:
	rm -rf out/
	mkdir out/
	gcc -c -DGCC ./test_bitrate.c
	gcc -o test_bitrate test_bitrate.o
	./test_bitrate mikro.bmp

cleanbitrate:
	rm -f test_bitrate test_bitrate.o

doc: doxy

doxy:
	doxygen doxygen.config
	man doc/man/man3/test_bitrate.c.3

doc-clean:
	rm -rf doc/*

clean:
	cd lib/jxrlib-master/ && $(MAKE) clean
	cd lib/jpeg-9a/ && $(MAKE) clean
	cd lib/openjpeg-2.1.0/ && $(MAKE) clean
	cd lib/iqa/ && $(MAKE) clean RELEASE=1
