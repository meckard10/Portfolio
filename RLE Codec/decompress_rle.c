/* decompress_rle.c
 * Melody Eckard
 *
 * Run Length Encoding (RLE) decompressor.
 *
 * Takes compressed RLE data and converts back to original file.
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	FILE		*rlefpt, *fpt;
	int		i, j, k;
	u_int8_t	count;
	unsigned char	symbol;


	if(argc != 3){
		printf("Provide rle and output files\n");
		exit(0);
	}

	rlefpt = fopen(argv[1], "r");
	if(rlefpt == NULL){
		printf("Unable to open %s for reading\n", argv[1]);
		exit(0);
	}

	fpt = fopen(argv[2], "w");
	if(fpt == NULL){
		printf("Unable toopen %s for writing\n", argv[2]);
		exit(0);
	}

	while(1){
		i = fread(&count, 1, 1, rlefpt);
		j = fread(&symbol, 1, 1, rlefpt);
		if(i < 1 || j < 1) break;
		
		for(k = 0; k <= count; k++){
			fwrite(&symbol, 1, 1, fpt);
		}
	}




	fclose(rlefpt);
	fclose(fpt);
}
