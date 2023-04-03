#include "config.h"

#ifdef FBV_SUPPORT_PNG
#include "fbv.h"
#include <stdlib.h>
#include <string.h>
#include <spng.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define PNG_BYTES_TO_CHECK 4
#ifndef min
#define min(x,y) ((x) < (y) ? (x) : (y))
#endif

int fh_png_id(char *name)
{
	int fd;
	char id[4];
	fd = open(name, O_RDONLY);
	if(fd==-1) return(0);
	read(fd, id, 4);
	close(fd);
	if(id[1]=='P' && id[2]=='N' && id[3]=='G') return(1);
	return(0);
}


int fh_png_load(char *name, unsigned char *buffer, unsigned char ** alpha,int x,int y)
{
	spng_ctx *ctx = spng_ctx_new(0);
	// png_structp png_ptr;
	// png_infop info_ptr;
	// png_uint_32 width, height;
	// png_uint_32 i;
	int bit_depth, color_type, interlace_type;
	int number_passes,pass, trans = 0;
	// png_bytep rptr[2];
	unsigned char *rp;
	unsigned char *fbptr;
	FILE *fh;

	if(!(fh = fopen(name,"rb"))) {
		spng_ctx_free(ctx);
		return(FH_ERROR_FILE);
	}

	//read entire opened file into memeory
	fseek(fh, 0, SEEK_END);
	long fsize = ftell(fh);
	fseek(fh, 0, SEEK_SET);  //same as rewind(f);

	char *file_data = malloc(fsize + 1);
	fread(file_data, fsize, 1, fh);
	fclose(fh);

	//decode png
	spng_set_png_buffer(ctx, file_data, fsize);

	size_t size;
	spng_decoded_image_size(ctx, SPNG_FMT_RGB8, &size);

	unsigned int width;
	unsigned int height;
	spng_get_image_limits(ctx, &width, &height);
	
	spng_decode_image(ctx, buffer, size, SPNG_FMT_RGB8, 0);

	free(file_data);

	spng_ctx_free(ctx);

	return(FH_ERROR_OK);
}


int fh_png_getsize(char *name, int *x, int *y)
{
	spng_ctx *ctx = spng_ctx_new(0);
	FILE *fh;

	if(!(fh = fopen(name,"rb"))) {
		spng_ctx_free(ctx);
		return(FH_ERROR_FILE);
	}

	//read entire opened file into memeory
	fseek(fh, 0, SEEK_END);
	long fsize = ftell(fh);
	fseek(fh, 0, SEEK_SET);  //same as rewind(f);

	char *file_data = malloc(fsize + 1);
	fread(file_data, fsize, 1, fh);
	fclose(fh);

	//decode png
	spng_set_png_buffer(ctx, file_data, fsize);

	unsigned int width;
	unsigned int height;
	spng_get_image_limits(ctx, &width, &height);

	struct spng_ihdr ihdr;

	spng_get_ihdr(ctx, &ihdr);

	*x = (int)ihdr.width;
	*y = (int)ihdr.height;
	
	// spng_decode_image(ctx, buffer, size, SPNG_FMT_RGB8, 0);

	free(file_data);

	spng_ctx_free(ctx);

	return(FH_ERROR_OK);
}
#endif /*FBV_SUPPORT_PNG*/

