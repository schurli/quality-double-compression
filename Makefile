
all:
	cd lib/jxrlib-master/ && $(MAKE)
	cd lib/jpeg-9a/ && ./configure && $(MAKE)
	cd lib/openjpeg-2.1.0/ && cmake . && $(MAKE)
	cd lib/iqa/ && $(MAKE) RELEASE=1
clean:
	cd lib/jxrlib-master/ && $(MAKE) clean
	cd lib/jpeg-9a/ && $(MAKE) clean
	cd lib/openjpeg-2.1.0/ && $(MAKE) clean
	cd lib/iqa/ && $(MAKE) clean RELEASE=1
