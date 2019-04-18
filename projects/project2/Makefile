make:
	g++ alloc.cpp -o alloc.out -pthread -lc
	g++ prov-rep.cpp -o prov-rep.out -pthread -lc && ./prov-rep.out

clean:
	rm -f alloc.out
	rm -f prov-rep.out
