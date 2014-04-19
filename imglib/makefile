all: imglib move

imglib: Image.cpp Matrix.cpp
	g++ Image.cpp Matrix.cpp -idirafter include -L/usr/local/lib -c

move:
	mkdir -p obj
	mv *.o obj/

clean:
	rm -rf obj
