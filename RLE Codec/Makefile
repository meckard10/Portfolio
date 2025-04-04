CC	= gcc

BINS	= compress_rle decompress_rle

all: $(BINS)

%: %.c
	$(CC) -o $@ $^ -g #-lX11

clean:
	$(RM) $(BINS)
