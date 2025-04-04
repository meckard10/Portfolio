CC	= clang
CFLAGS	= -Wall -g


BINS	= libmyalloc.so

all: $(BINS)

%.out: %.c
	$(CC) $(CFLAGS) -o $@ $^

#development
#libmyalloc.so: libmyalloc.c
#	$(CC) $(CFLAGS) -shared -fPIC libmyalloc.c -o libmyalloc.so

#optimized for performance
libmyalloc.so: libmyalloc.c
	$(CC) -O2 -DNDEBUG -Wall -fPIC -shared libmyalloc.c -o libmyalloc.so

clean:
	$(RM) $(BINS) 
	$(RM) -r *.dSYM
