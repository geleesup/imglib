caResize: CA_Resize.cpp Shrink.cpp oldStretch \
energyMap.cpp kminSeam.cpp mat2im.cpp minSeam.cpp minSeam2.cpp
	g++ CA_Resize.cpp Shrink.cpp stretch.o \
    energyMap.cpp kminSeam.cpp mat2im.cpp minSeam.cpp minSeam2.cpp \
    imglib/obj/*.o -idirafter imglib/include -L/usr/local/lib -o caResize

newStretch:
	g++ Stretch2.cpp -idirafter imglib/include -c -o stretch.o

oldStretch:
	g++ Stretch.cpp -idirafter imglib/include -c -o stretch.o

clean:
	rm -f caResize
	rm -f stretch.o
