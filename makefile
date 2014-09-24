#W=wine /home/cuki/.wine/drive_c/Program\ Files/PICC/
CC=Ccsc.exe
CFLAGS=+FH +LN -T -A -M -Z +DF +Y=9 +STDOUT

all: l298n

l298n: l298n.c
	$(CC) $(CFLAGS) l298n.c
#$(W)$(CC) $(CFLAGS) l298n.c

clean:
	rm *.cof *.err *.esym *.hex *.lst *.PWI *.STA *.sym *.tre *.DBK