/* compress_rle.c
 * Melody Eckard
 *
 * Run Length Encoding (RLE) compressor.
 *
 * Can be used on ppm images, text files, and binary executables.
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	FILE		*fpt, *newfpt;
	u_int8_t	RunCount;
	int		i;
	unsigned char	A, B;
	
	
	if(argc != 3){
		printf("Provide input and output file\n");
		exit(0);
	}

	fpt = fopen(argv[1], "r");
	if(fpt == NULL){
		printf("Unable to open %s for reading\n", argv[1]);
		exit(0);
	}

	newfpt = fopen(argv[2], "w");
	if(newfpt == NULL){
		printf("Unable to open %s for writing\n", argv[2]);
		exit(0);
	}

	RunCount = 0;

	i = fread(&A, 1, 1, fpt);

	while(1){
		i = fread(&B, 1, 1, fpt);
		if(i < 1){
			break;
		}

		if((A == B) && (RunCount < 255)){
			RunCount++;
		}
		else{
			fwrite(&RunCount, 1, 1, newfpt);
			fwrite(&A, 1, 1, newfpt);
			A = B;
			RunCount = 0;
		}

	}
	
	fwrite(&RunCount, 1, 1, newfpt);
	fwrite(&A, 1, 1, newfpt);

	fclose(fpt);
	fclose(newfpt);
}

