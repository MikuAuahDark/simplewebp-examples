/* clang -o webp2png -D_CRT_SECURE_NO_WARNINGS webp2png.c lodepng.c */
#include <stdio.h>

#include "lodepng.h"

#define SIMPLEWEBP_IMPLEMENTATION
#include "../simplewebp/simplewebp.h"

int main(int argc, char *argv[])
{
	unsigned char *rgba;
	unsigned int lpng_err;
	simplewebp *swebp;
	simplewebp_error swebp_err;
	size_t width, height;

	if (argc < 3)
	{
		fprintf(stderr, "Usage: %s <input.webp> <output.png>\n", argv[0]);
		return 1;
	}

	swebp_err = simplewebp_load_from_filename(argv[1], NULL, &swebp);
	if (swebp_err != SIMPLEWEBP_NO_ERROR)
	{
		fprintf(stderr, "%s: %s\n", argv[1], simplewebp_get_error_text(swebp_err));
		return 1;
	}

	simplewebp_get_dimensions(swebp, &width, &height);
	rgba = calloc(4, width * height);
	if (!rgba)
	{
		simplewebp_unload(swebp);
		fprintf(stderr, "%s: Not enough memory to allocate %u bytes\n", argv[1], (unsigned int) (width * height));
		return 1;
	}

	swebp_err = simplewebp_decode(swebp, rgba, NULL);
	if (swebp_err != SIMPLEWEBP_NO_ERROR)
	{
		simplewebp_unload(swebp);
		free(rgba);
		fprintf(stderr, "%s: %s\n", argv[1], simplewebp_get_error_text(swebp_err));
		return 1;
	}

	simplewebp_unload(swebp);

	lpng_err = lodepng_encode32_file(argv[2], rgba, (unsigned int) width, (unsigned int) height);
	if (lpng_err != 0)
	{
		free(rgba);
		fprintf(stderr, "%s: %s\n", argv[2], lodepng_error_text(lpng_err));
		return 1;
	}

	free(rgba);
	return 0;
}
