CC=g++
CFLAGS=-Wall -pedantic -std=c++11
LINKFLAGS=-L "C:\Program Files\Microsoft SDKs\Windows\v7.0\Lib" -lwinMM

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(LINKFLAGS)

rejfpank-visuals:
	$(CC) -o rejfpank-visuals main.cpp $(CFLAGS) $(LINKFLAGS)

clean:
	rm rejfpank-visuals