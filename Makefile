
all:
	cd lib/jxr-1.1/ && $(MAKE)
	cd lib/jpg-9a/ && ./configure && $(MAKE)
	cd lib/j2k-2.1.0/ && cmake . && $(MAKE)
	cd lib/vqi-0.3.3/ && $(MAKE) all
	cd lib/iqa-1.1.2/ && $(MAKE) RELEASE=1

doc: doxy

doxy:
	doxygen doxygen.config
	man doc/man/man3/test_bitrate.c.3

doc-clean:
	rm -rf doc/*

dcprocess:
	rm -rf out/
	mkdir out/
	gcc -c -DGCC -static src/dcprocess.c src/coder.c src/metrics.c src/bmp.c
	gcc -lm -o dcprocess dcprocess.o coder.o metrics.o bmp.o -Llib/iqa-1.1.2/build/release -liqa

	#./dcprocess -i test-db -o out -a "0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95" -b "0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65"
	#./dcprocess -i test-db -o out -a "0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65"
	#./dcprocess -i one -o out -a "0.005,0.01,0.015,0.02,0.025,0.03,0.035,0.04,0.045,0.05,0.055,0.06,0.065,0.07,0.075,0.08,0.085,0.09,0.095,0.1"
	./dcprocess -i one -o out -a "0.005"

dcprocess-clean:
	rm -f dcprocess dcprocess.o coder.o metrics.o

clean:
	cd lib/jxr-1.1/ && $(MAKE) clean
	cd lib/jpg-9a/ && $(MAKE) clean
	cd lib/j2k-2.1.0/ && $(MAKE) clean
	cd lib/vqi-0.3.3/ && $(MAKE) clean
	cd lib/iqa-1.1.2/ && $(MAKE) clean RELEASE=1
