all: fake

fake: fake.c
	gcc -o fake fake.c

run: server.py fake
	./server.py &
	./fake

clean:
	rm -rf fake *~

.PHONY: all clean
