CC=ccsc
CFLAGS=+FH +LN -T -A -M -Z +DF +Y=9 +STDOUT

all: l298n

l298n: l298n.c
	$(CC) $(CFLAGS) l298n.c

clean:
	rm *.cof *.err *.esym *.hex *.lst