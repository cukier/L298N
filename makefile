CC=ccsc
CFLAGS=+FH +LN -T -A -M -Z +DF +Y=9 +STDOUT

all: l298n compare

l298n: l298n.c
	$(CC) $(CFLAGS) l298n.c

compare: compare.c
	$(CC) $(CFLAGS) compare.c

clean:
	rm *.cof *.err *.esym *.hex *.lst