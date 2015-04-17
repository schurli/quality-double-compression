
all:
	cd lib/jxr-1.1/ && $(MAKE)
	cd lib/jpg-9a/ && ./configure && $(MAKE)
	cd lib/j2k-2.1.0/ && cmake . && $(MAKE)
	cd lib/vqi-0.3.3/ && $(MAKE) all
	cd lib/iqa-1.1.2/ && $(MAKE) RELEASE=1

bitrate:
	rm -rf out/
	mkdir out/
	gcc -c -DGCC ./test_bitrate.c
	gcc -o test_bitrate test_bitrate.o
	./test_bitrate cat.bmp 0.17

bitrate-clean:
	rm -f test_bitrate test_bitrate.o

doc: doxy

doxy:
	doxygen doxygen.config
	man doc/man/man3/test_bitrate.c.3

doc-clean:
	rm -rf doc/*

clean:
	cd lib/jxr-1.1/ && $(MAKE) clean
	cd lib/jpg-9a/ && $(MAKE) clean
	cd lib/j2k-2.1.0/ && $(MAKE) clean
	cd lib/vqi-0.3.3/ && $(MAKE) clean
	cd lib/iqa-1.1.2/ && $(MAKE) clean RELEASE=1
