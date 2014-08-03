.PHONY: all clean

all: hexcom

clean:
	rm -f hexcom

hexcom: hexcom.c
	gcc -std=gnu99 -Wall -O2 -ohexcom hexcom.c
