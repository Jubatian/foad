/*  GIMP header image file format (INDEXED): /home/pettyes/uzebox/my_devel/tutor_test/dragon/assets/dragon_sprites.h  */

static unsigned int width = 1248;
static unsigned int height = 8;

/*  Call this macro repeatedly.  After each use, the pixel data can be extracted  */

#define HEADER_PIXEL(data,pixel) {\
pixel[0] = header_data_cmap[(unsigned char)data[0]][0]; \
pixel[1] = header_data_cmap[(unsigned char)data[0]][1]; \
pixel[2] = header_data_cmap[(unsigned char)data[0]][2]; \
data ++; }

static char header_data_cmap[256][3] = {
	{  0,  0,  0},
	{109, 36,  0},
	{ 36, 73,  0},
	{ 36,109,  0},
	{109,109, 85},
	{109,146,  0},
	{146,219,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255}
	};
static char header_data[] = {
	6,6,6,6,6,6,6,6,6,6,6,6,5,5,5,5,
	6,6,6,6,5,6,6,6,5,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,3,3,3,0,0,0,6,
	0,0,0,0,4,3,2,6,6,6,6,6,6,5,5,6,
	6,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,
	6,6,6,2,0,0,4,3,2,0,0,6,2,3,3,0,
	5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	6,6,6,6,5,6,6,6,5,6,6,6,6,6,6,6,
	6,6,5,3,3,0,0,6,0,4,3,2,0,0,2,6,
	5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	6,6,6,6,6,5,5,6,6,5,5,5,5,5,6,6,
	6,6,6,5,3,2,0,0,2,0,0,6,0,4,3,2,
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	6,6,6,6,5,6,6,6,5,5,5,5,5,6,6,6,
	6,3,3,3,0,0,0,6,6,6,0,0,3,3,2,6,
	5,3,2,2,4,5,5,6,2,3,6,6,6,6,6,6,
	6,6,6,6,5,6,6,6,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,3,3,6,6,6,6,6,
	3,2,2,3,4,5,6,6,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,5,6,6,6,6,6,6,5,3,
	5,6,6,6,5,5,6,6,6,6,5,6,6,6,6,6,
	6,6,5,6,6,6,6,6,1,5,1,2,2,6,6,6,
	6,6,6,6,6,6,6,6,6,1,5,1,6,6,6,6,
	6,6,5,2,0,3,2,6,6,6,6,5,6,6,6,6,
	5,3,3,2,0,2,0,6,5,5,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,5,5,6,5,5,5,6,6,
	6,6,6,6,6,6,6,6,6,3,2,2,0,2,2,3,
	6,6,5,6,6,6,6,6,0,0,2,0,0,3,4,2,
	6,6,6,6,6,2,3,5,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	2,2,3,4,6,6,6,6,3,2,2,3,2,0,0,0,
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	2,2,2,2,3,6,6,6,3,3,4,3,3,0,0,0,
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,0,2,3,3,3,3,0,
	2,5,3,3,3,2,0,0,5,5,5,5,5,3,6,5,
	5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	3,4,3,3,6,6,6,6,6,6,6,6,6,6,6,6,
	6,6,5,5,5,5,5,5,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,6,6,5,3,3,
	3,2,6,6,6,6,6,6,6,3,3,3,0,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,6,6,0,4,3,
	6,6,3,2,3,2,2,0,0,5,3,3,3,3,3,6,
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,2,4,3,3,3,3,0,0,
	6,6,6,5,3,2,0,6,6,6,6,6,6,6,6,6,
	6,6,6,6,5,6,6,6,6,6,6,6,6,6,6,6,
	6,6,5,3,3,6,6,6,2,3,3,4,5,5,4,6,
	5,5,6,6,6,6,6,6,5,5,6,6,6,6,6,6,
	6,6,6,6,6,5,6,6,6,3,5,5,3,5,6,6,
	4,3,2,2,3,5,6,6,3,2,0,2,2,3,5,3,
	5,6,6,6,6,6,6,6,6,6,6,6,5,6,6,6,
	3,0,2,3,4,5,6,6,3,4,3,2,2,5,6,6,
	6,6,6,5,6,6,6,6,3,0,2,3,5,5,6,6,
	6,0,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	6,6,6,6,4,6,6,6,6,6,6,6,6,6,6,6,
	6,6,3,5,6,6,6,6,6,6,5,6,6,6,6,6,
	6,5,6,6,6,6,6,6,3,5,3,3,3,6,6,6,
	6,6,2,3,3,4,5,6,6,6,6,6,6,6,6,6,
	6,0,2,2,2,2,5,3,6,6,5,2,0,6,6,6,
	5,3,3,2,5,6,6,6,5,6,6,6,6,6,6,6,
	6,6,6,6,5,6,6,6,6,6,6,6,6,6,6,6,
	6,6,5,3,3,0,0,6,6,6,6,4,3,3,0,6,
	6,6,6,6,6,5,3,6,6,6,5,5,3,5,3,3,
	6,6,6,5,5,5,6,6,6,5,5,5,6,6,6,6,
	6,6,5,3,3,2,6,6,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,5,5,
	6,6,6,6,6,5,5,5,6,6,6,5,3,3,2,0,
	6,6,6,6,6,6,6,5,6,6,6,6,6,6,6,5,
	6,5,3,3,2,6,6,6,6,6,6,6,6,6,6,6,
	6,3,5,5,3,5,6,6,6,3,5,5,3,5,6,6,
	6,6,6,6,6,6,6,5,6,3,5,5,3,5,6,6,
	6,3,5,5,3,5,6,6,6,3,5,5,3,5,6,6,
	6,6,6,6,6,6,5,6,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,2,2,2,0,0,3,3,4,
	6,6,6,2,3,5,5,6,6,3,4,3,4,4,6,6,
	6,6,6,6,6,6,6,6,6,5,5,4,3,2,6,6,
	6,6,5,5,3,5,5,6,4,5,5,5,5,5,6,6,
	5,3,2,2,4,5,5,6,6,6,3,3,0,6,6,6,
	6,0,0,0,3,3,3,6,6,6,6,6,5,5,3,5,
	5,5,4,4,3,3,5,5,5,3,2,2,3,4,5,6,
	6,6,6,2,0,0,5,3,0,6,6,6,2,3,3,0,
	3,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	6,6,5,5,3,5,5,6,4,5,5,5,5,5,6,6,
	6,6,3,3,2,6,6,6,6,4,3,2,0,2,2,6,
	3,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	6,6,6,6,5,3,3,5,5,5,4,4,3,3,5,5,
	6,6,6,3,3,2,0,0,0,6,6,6,0,3,3,2,
	5,3,2,2,4,5,5,6,6,6,6,6,6,6,6,6,
	6,6,6,5,3,5,5,6,4,4,5,3,3,5,5,6,
	6,6,3,3,6,6,6,6,6,6,2,0,2,3,3,6,
	5,5,5,4,3,2,6,6,2,2,6,6,6,6,6,6,
	6,6,6,5,3,5,6,6,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,2,3,6,6,6,6,6,
	3,0,2,4,5,5,4,6,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,6,6,5,3,3,
	4,5,5,5,5,5,5,6,6,5,4,6,6,6,6,6,
	6,6,5,6,6,6,6,6,3,5,3,0,2,5,5,5,
	6,6,6,6,6,6,6,6,6,3,5,3,2,3,6,6,
	6,6,3,2,6,3,3,6,6,6,6,5,6,6,6,6,
	2,5,3,3,2,3,0,6,6,4,5,5,6,6,6,6,
	6,6,6,6,6,6,6,6,3,5,5,3,3,5,5,6,
	6,6,6,6,6,6,6,6,6,2,0,2,0,2,6,2,
	6,6,3,5,6,6,6,6,0,2,0,0,0,2,3,2,
	6,6,6,6,6,6,2,5,5,3,2,3,4,5,5,6,
	6,6,6,6,5,5,6,6,6,6,6,6,6,6,6,6,
	6,2,2,3,6,6,6,6,4,3,2,5,3,0,2,6,
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	6,2,0,2,2,6,6,6,3,5,3,3,2,0,0,0,
	5,4,3,2,3,5,5,6,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,0,2,2,4,3,2,0,
	0,2,4,3,2,0,6,6,3,5,4,3,3,5,5,4,
	3,3,4,5,5,5,5,5,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	0,3,5,3,3,6,6,6,5,3,2,2,4,5,5,6,
	5,5,5,4,5,3,3,3,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,5,4,3,3,3,
	2,0,6,6,6,6,6,6,6,3,3,3,3,3,6,6,
	5,4,3,2,4,5,5,6,6,6,6,6,5,5,5,5,
	6,6,6,6,6,6,6,6,6,6,6,6,6,5,3,3,
	6,6,2,2,3,2,0,0,6,6,6,6,2,3,2,6,
	5,3,2,3,4,5,5,6,6,6,6,6,5,5,5,6,
	6,6,6,6,6,6,6,6,2,2,4,3,3,2,0,0,
	6,6,6,3,4,3,0,2,6,6,6,6,6,6,6,6,
	6,6,6,5,3,5,6,6,6,6,6,6,6,6,6,6,
	6,6,5,3,3,6,6,6,0,2,3,5,3,3,5,4,
	3,3,1,5,5,6,6,6,5,2,4,5,5,6,6,6,
	6,6,6,6,5,3,5,6,2,3,3,5,5,2,5,6,
	5,3,2,2,3,5,6,6,3,2,6,0,2,5,3,3,
	3,5,6,6,6,6,6,6,6,6,6,3,5,6,6,6,
	3,2,0,3,3,5,6,6,4,2,5,3,2,4,6,6,
	6,6,6,3,5,6,6,6,3,2,0,4,3,5,6,6,
	6,2,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	6,6,6,6,3,5,6,6,6,6,6,6,6,6,6,6,
	6,5,5,5,5,6,6,6,6,6,5,6,6,6,6,6,
	6,4,5,6,6,6,6,6,3,5,3,3,2,6,6,6,
	6,6,2,3,2,3,5,5,6,6,6,6,6,6,6,6,
	6,0,0,0,0,0,5,3,6,6,3,2,0,6,6,6,
	5,3,3,2,3,5,6,6,3,5,6,6,6,6,6,6,
	6,6,6,5,3,5,5,5,5,5,6,6,6,6,6,6,
	6,6,3,3,2,6,6,6,6,6,6,6,5,3,0,6,
	6,6,6,6,5,3,3,3,5,5,4,3,3,4,3,3,
	6,5,5,4,3,4,5,5,6,3,3,3,5,5,5,6,
	6,6,3,3,2,6,6,6,6,6,6,2,6,6,6,6,
	6,6,6,6,5,5,6,6,6,5,5,5,5,4,3,3,
	6,6,5,5,5,3,6,6,6,6,5,4,3,2,0,0,
	6,6,6,6,6,5,4,3,6,6,6,6,6,6,5,3,
	5,3,2,2,6,6,0,0,6,6,6,6,6,6,6,6,
	2,3,3,5,5,2,5,6,2,3,3,5,5,2,5,6,
	6,6,6,6,6,5,5,4,2,3,3,5,5,2,5,6,
	2,3,3,5,5,2,5,6,2,3,3,5,5,2,5,6,
	6,6,6,6,4,5,3,5,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,0,2,3,3,4,5,
	6,6,6,3,3,4,5,6,6,3,4,2,3,5,6,6,
	6,6,6,6,5,6,6,6,6,6,6,6,6,6,6,6,
	6,5,5,3,3,4,4,6,3,3,4,5,3,3,5,6,
	5,5,5,4,3,2,6,6,6,2,3,2,0,6,6,6,
	6,2,0,6,6,2,3,6,6,6,6,5,5,3,3,3,
	4,3,3,3,5,3,3,3,5,5,5,4,5,3,6,6,
	6,6,6,3,0,6,3,3,3,6,6,6,6,2,3,2,
	2,3,5,6,6,6,6,6,6,6,6,6,6,6,6,6,
	6,4,5,3,3,4,4,6,3,3,5,5,3,3,5,6,
	6,3,3,2,2,6,6,6,6,3,3,6,6,0,2,6,
	3,3,5,6,6,6,6,6,6,6,6,6,6,6,6,6,
	6,6,6,5,3,3,3,3,4,3,3,3,5,3,3,3,
	6,6,6,3,3,6,0,0,0,6,6,6,6,2,3,0,
	5,5,5,4,3,2,6,6,6,6,6,6,6,6,6,6,
	6,6,5,3,3,4,4,6,3,3,3,5,3,3,3,6,
	6,6,3,2,6,6,6,6,6,6,2,0,0,2,3,2,
	3,5,3,3,5,4,3,6,0,2,6,6,6,6,6,6,
	6,6,5,3,3,4,6,6,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,3,6,6,6,6,6,
	2,0,2,5,3,3,5,4,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,6,5,3,3,3,
	3,4,4,5,3,3,5,4,6,5,2,2,6,6,6,6,
	6,6,5,6,6,6,6,6,3,3,3,0,2,3,3,4,
	6,6,6,6,6,6,6,6,6,3,3,3,2,3,5,5,
	6,6,3,2,6,5,3,6,6,6,6,3,6,6,6,6,
	0,2,5,3,2,3,3,6,5,3,2,4,5,6,6,6,
	6,6,6,6,6,6,6,6,3,3,4,3,3,4,5,5,
	6,6,6,6,6,6,6,6,6,5,2,0,2,3,6,6,
	6,6,2,3,5,5,6,6,0,2,0,0,0,2,3,3,
	6,6,6,6,6,3,2,5,5,5,5,5,3,2,6,6,
	6,5,5,5,5,3,5,5,6,6,6,6,6,6,6,6,
	6,2,2,2,6,6,6,6,2,2,2,4,3,0,2,6,
	6,6,5,5,5,5,6,6,6,6,6,6,6,6,6,6,
	6,6,0,0,2,6,6,6,2,5,3,3,2,6,6,6,
	5,5,5,5,4,2,6,6,6,5,5,5,5,5,6,6,
	6,6,6,6,6,6,6,6,3,2,0,0,5,3,2,6,
	0,0,3,3,3,6,6,6,3,3,4,5,5,5,4,3,
	2,3,3,3,3,5,3,3,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,3,0,5,3,3,6,
	0,0,0,3,3,3,6,6,5,5,5,5,3,2,6,6,
	4,4,3,3,3,5,3,3,5,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,5,4,3,3,3,2,0,
	2,0,0,6,6,6,6,6,6,6,0,2,3,3,6,6,
	5,5,5,5,3,2,6,6,6,6,5,5,5,3,3,4,
	5,5,6,6,6,6,6,6,6,6,6,6,5,3,3,2,
	6,6,0,0,2,0,0,6,6,6,6,6,5,2,0,6,
	5,5,5,5,3,2,6,6,6,6,5,5,5,5,4,5,
	5,6,6,6,6,6,6,6,6,2,2,4,3,0,0,0,
	6,6,6,6,5,3,0,2,6,6,6,6,6,6,6,6,
	6,6,5,3,3,5,6,6,6,6,6,6,6,6,6,6,
	6,6,3,3,0,6,6,6,0,2,2,5,3,3,3,5,
	3,3,3,3,5,6,6,6,5,5,3,2,3,5,5,6,
	6,6,6,5,3,3,5,6,2,2,3,3,5,5,3,6,
	3,3,3,2,3,4,5,6,3,6,6,0,5,3,3,3,
	3,5,6,6,6,6,6,6,6,6,4,5,3,5,6,6,
	2,2,0,2,5,3,5,6,4,2,5,3,2,3,5,6,
	5,3,0,3,5,6,6,6,2,2,0,5,3,3,5,6,
	6,2,6,6,6,6,6,6,0,3,5,6,6,6,6,6,
	2,3,6,0,3,3,5,6,6,6,6,6,6,6,6,6,
	3,4,5,3,5,6,6,6,6,6,4,5,6,6,6,6,
	6,3,4,5,6,6,6,6,4,3,4,3,2,6,6,6,
	6,6,0,2,2,3,3,5,6,6,6,6,6,6,6,6,
	6,6,6,6,0,0,6,6,6,6,6,3,0,6,6,6,
	6,5,3,0,2,3,5,6,2,3,5,6,6,6,6,6,
	6,6,5,3,3,4,4,4,4,5,5,5,5,6,6,6,
	6,4,3,2,2,6,6,6,6,6,6,6,5,3,2,0,
	6,6,6,5,3,3,3,2,6,4,3,3,2,0,5,3,
	6,6,6,6,6,6,4,3,6,0,2,2,3,3,3,5,
	6,4,3,2,2,6,6,6,6,6,6,3,2,0,6,6,
	6,6,6,5,5,3,6,6,4,3,3,3,3,3,2,2,
	5,4,3,2,6,6,6,6,6,6,6,3,2,2,0,6,
	6,6,6,6,5,3,2,6,6,6,6,6,6,5,3,6,
	3,2,0,6,0,2,2,0,6,6,6,6,6,6,6,6,
	2,2,3,3,5,5,3,6,2,3,5,3,5,5,3,6,
	6,6,6,6,6,5,4,3,2,3,5,3,5,5,6,6,
	2,3,5,3,5,5,3,6,2,3,5,3,5,5,3,6,
	6,6,6,4,5,3,3,4,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,0,2,3,4,4,
	6,6,2,3,3,3,5,6,2,3,6,0,3,5,6,6,
	6,6,5,3,5,6,6,6,6,6,6,6,6,6,6,6,
	6,5,3,3,3,3,3,6,3,3,3,5,3,3,3,6,
	3,5,3,5,5,5,5,6,4,0,3,3,6,6,6,6,
	6,2,2,6,6,3,3,3,6,6,3,3,5,3,3,3,
	3,3,2,5,3,3,3,3,3,5,3,5,5,3,3,6,
	6,6,4,3,2,6,6,3,2,6,6,6,6,2,3,2,
	0,3,3,5,6,6,6,6,6,6,6,6,6,6,6,6,
	6,5,3,3,3,3,3,6,3,2,5,3,3,3,3,6,
	3,2,2,0,6,6,6,6,6,3,2,6,6,2,2,3,
	2,3,3,5,6,6,6,6,6,6,6,6,6,6,6,6,
	6,6,4,5,3,3,3,3,3,3,2,3,5,3,3,3,
	6,6,4,3,2,6,6,0,2,6,6,6,6,2,3,0,
	3,5,3,3,5,5,3,6,2,2,3,3,6,6,6,6,
	6,5,3,3,3,3,3,6,3,2,3,5,3,3,3,6,
	6,6,3,3,6,6,6,6,6,6,2,2,6,2,3,2,
	3,3,3,5,3,5,4,6,0,2,6,6,6,6,6,6,
	6,4,3,3,3,3,6,6,5,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,3,6,6,6,6,6,
	3,0,0,5,3,3,3,5,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,0,3,3,3,2,
	2,3,3,5,3,3,3,5,5,5,5,2,6,6,6,6,
	6,5,5,5,6,6,6,6,5,3,2,0,3,3,2,3,
	6,6,6,6,6,6,6,6,5,2,3,0,2,3,3,3,
	6,6,3,0,6,5,3,6,6,6,5,3,6,6,6,6,
	2,6,6,3,2,3,2,6,6,4,5,5,5,3,5,6,
	6,5,6,6,6,6,6,6,3,2,3,2,3,3,5,3,
	6,6,6,6,5,6,6,6,6,3,0,0,3,3,6,6,
	6,6,6,6,2,3,5,5,6,2,2,0,0,2,2,3,
	6,6,6,6,2,3,4,6,3,5,3,3,5,5,5,5,
	5,4,3,3,5,3,3,4,5,5,6,6,6,6,6,6,
	6,6,6,0,6,6,6,6,0,2,5,3,0,0,6,6,
	5,5,4,3,5,3,5,6,5,6,6,6,6,6,6,6,
	6,6,2,0,6,6,6,6,2,5,3,2,6,6,6,6,
	3,5,4,3,3,5,5,5,5,4,3,3,3,3,5,5,
	6,6,6,6,6,6,6,6,3,0,6,6,5,3,6,2,
	0,0,0,3,3,6,6,6,2,0,3,3,4,5,3,3,
	2,3,3,2,3,5,3,3,5,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,3,2,2,4,2,6,6,
	6,0,0,0,0,3,2,6,3,5,3,3,5,5,5,5,
	3,3,3,2,3,5,3,3,4,4,5,4,6,6,6,6,
	6,6,6,6,6,6,6,6,3,3,3,2,0,2,0,6,
	6,6,6,6,6,6,6,6,6,6,6,0,0,3,3,6,
	3,5,4,3,5,5,3,3,6,5,5,3,5,3,3,3,
	3,2,3,5,5,5,5,6,6,6,6,5,3,3,2,0,
	6,6,0,0,0,0,6,6,6,6,6,6,3,2,0,0,
	3,5,3,4,5,5,3,6,5,5,4,3,5,3,3,3,
	3,5,5,6,6,6,6,6,6,6,0,3,3,6,0,0,
	6,6,6,0,5,2,0,2,6,6,6,6,6,6,6,6,
	6,5,3,3,3,3,5,6,6,6,6,6,6,6,6,6,
	6,6,3,3,6,6,6,6,0,0,2,4,3,2,3,3,
	6,2,3,3,3,6,6,6,6,6,6,6,6,3,4,5,
	6,6,3,5,3,3,3,6,6,2,3,5,3,5,5,6,
	6,5,3,0,3,3,5,6,2,6,6,6,5,3,3,2,
	2,3,5,6,6,6,6,6,6,3,4,4,3,5,6,6,
	6,3,0,2,4,3,5,6,4,3,4,3,2,3,5,6,
	5,3,2,2,3,5,6,6,6,3,0,5,3,3,5,6,
	3,3,6,6,6,6,6,6,6,2,3,5,6,6,6,6,
	3,3,3,6,0,3,3,5,6,6,6,6,6,6,6,6,
	2,5,4,3,3,6,6,6,6,6,3,5,6,6,6,6,
	6,3,3,4,5,5,6,6,4,2,3,2,2,6,6,6,
	6,6,0,0,3,2,5,3,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,3,0,6,6,6,
	6,5,3,2,0,2,3,5,6,2,3,5,6,6,6,6,
	6,5,3,3,3,3,3,3,3,3,5,3,5,5,6,6,
	4,2,2,0,6,6,6,6,6,6,6,6,6,3,3,0,
	6,6,0,5,3,3,2,2,6,6,3,2,2,0,4,3,
	6,6,6,6,6,6,3,2,6,6,6,2,2,2,2,2,
	4,2,2,0,6,6,6,6,6,4,3,3,3,2,0,6,
	6,6,6,5,3,3,6,6,3,2,0,0,2,2,2,0,
	3,2,6,6,6,6,6,6,6,6,6,6,3,2,0,6,
	6,6,6,5,3,2,6,6,6,6,6,6,6,3,6,6,
	3,0,0,2,3,3,3,3,6,6,6,6,6,6,6,6,
	6,2,3,5,3,5,5,3,6,2,3,5,3,5,5,5,
	6,6,6,6,6,5,3,3,6,2,3,5,3,5,5,5,
	6,2,3,5,3,5,5,5,6,2,3,5,3,5,5,5,
	6,6,2,5,3,3,3,3,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,2,3,3,4,4,
	6,6,2,3,3,3,5,6,3,2,6,2,3,5,6,6,
	6,5,1,3,2,6,6,6,6,6,6,6,6,6,6,6,
	6,5,3,3,3,2,3,6,3,2,3,4,3,3,3,6,
	3,3,3,3,5,3,6,6,3,0,3,2,6,6,6,6,
	6,2,0,6,6,6,3,3,6,0,2,3,4,3,2,2,
	3,3,2,5,3,3,3,2,3,3,3,5,3,5,5,6,
	6,6,3,2,6,6,6,3,3,6,6,6,6,3,2,0,
	6,2,3,3,5,5,6,6,6,6,6,6,6,6,6,6,
	6,5,3,3,3,2,3,6,3,2,4,3,3,2,3,6,
	3,2,0,0,6,6,6,6,6,3,0,6,6,6,2,2,
	0,2,3,3,5,6,6,6,6,6,6,6,6,6,6,6,
	6,0,5,3,3,3,2,2,3,3,2,2,5,3,3,2,
	6,6,3,2,6,6,6,2,3,6,6,6,6,0,3,2,
	3,3,3,5,3,5,5,6,2,3,2,2,6,6,6,6,
	0,5,3,3,3,2,3,6,3,2,2,4,3,3,2,6,
	6,6,3,2,6,6,6,6,6,6,3,2,6,6,3,3,
	2,0,3,3,5,3,4,6,0,0,6,6,6,6,6,6,
	2,5,3,3,3,2,5,6,3,4,5,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,3,6,6,6,6,6,
	2,0,0,4,3,2,3,3,3,3,3,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,2,2,3,3,2,
	3,3,2,4,3,2,3,3,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,5,3,3,3,3,3,2,
	6,6,6,6,6,6,6,6,3,3,2,0,2,2,3,3,
	6,6,3,0,6,3,2,6,6,6,3,2,6,6,6,6,
	2,6,6,5,3,2,0,6,6,2,5,3,3,3,1,6,
	5,5,5,3,3,2,0,6,2,2,3,2,3,3,5,3,
	6,6,6,6,3,5,6,6,3,2,0,0,3,2,2,6,
	6,6,6,6,6,6,6,6,6,2,2,0,0,6,2,3,
	6,6,6,6,2,6,6,6,3,3,4,5,3,3,5,5,
	3,3,3,5,3,3,3,3,3,3,5,5,6,6,6,6,
	6,6,6,6,6,6,6,6,6,5,3,2,2,0,0,6,
	4,3,3,5,3,3,3,5,4,5,5,6,6,6,5,6,
	6,6,3,2,0,6,6,6,5,3,3,0,2,2,6,6,
	3,3,3,5,4,3,5,3,3,3,2,3,3,5,4,3,
	5,5,4,6,6,6,6,6,3,2,6,6,3,3,6,2,
	0,6,6,2,3,2,6,6,6,6,2,2,3,4,4,3,
	3,2,3,2,2,2,5,3,3,4,5,5,5,5,5,4,
	6,6,6,6,6,6,6,6,4,3,2,6,3,2,6,6,
	6,6,2,0,0,3,3,6,3,3,4,5,3,5,3,3,
	2,3,3,2,2,5,3,3,3,3,3,3,5,5,5,5,
	5,5,5,5,6,6,6,6,6,6,6,3,3,0,6,6,
	6,6,6,6,6,6,6,6,6,6,6,6,2,0,3,6,
	3,3,4,5,3,5,5,5,5,4,3,3,5,3,3,3,
	2,2,2,2,3,3,3,4,6,6,5,3,2,6,6,6,
	6,6,6,6,2,0,6,6,6,6,6,6,6,3,6,0,
	3,3,5,3,3,5,5,5,5,3,3,5,3,3,3,2,
	2,3,3,4,5,5,6,6,6,6,5,3,2,6,6,0,
	6,6,6,3,2,6,2,6,6,6,5,5,5,6,6,6,
	2,5,3,3,3,2,5,6,6,6,6,6,6,6,6,6,
	6,6,3,2,6,6,6,6,6,0,0,0,3,3,2,2,
	6,6,2,2,3,6,6,6,6,6,6,6,6,6,3,6,
	6,2,5,3,3,3,2,5,6,6,2,3,5,3,5,6,
	6,3,2,2,2,3,4,5,0,6,6,6,6,2,3,3,
	2,2,3,5,6,6,6,6,2,2,5,3,3,4,6,6,
	6,2,0,2,5,3,3,5,6,4,5,3,2,3,5,6,
	6,3,3,0,3,4,6,6,6,2,0,4,3,2,2,5,
	6,3,2,6,6,6,6,6,6,6,2,3,5,6,6,6,
	6,3,3,6,6,2,3,3,5,6,6,6,6,6,6,6,
	2,5,3,3,3,6,6,6,6,6,3,4,5,6,6,6,
	6,2,3,3,3,4,5,5,6,3,5,3,0,6,6,6,
	6,6,0,2,0,2,4,3,6,6,6,6,6,6,5,6,
	6,6,6,6,6,6,6,6,6,6,6,3,2,6,6,6,
	6,6,3,3,0,0,2,3,6,6,2,3,5,5,6,6,
	0,5,3,3,3,2,3,3,2,3,4,3,3,5,5,6,
	3,2,0,6,6,6,6,6,6,6,6,6,6,5,3,2,
	6,6,2,4,3,3,2,3,6,6,2,2,0,6,4,3,
	6,6,6,6,6,6,3,0,6,6,6,6,6,6,6,6,
	3,2,0,6,6,6,6,6,6,6,6,2,2,3,2,6,
	6,6,5,3,4,3,6,6,3,3,2,2,3,3,3,3,
	6,6,6,6,6,6,6,6,6,6,6,6,2,0,6,6,
	6,6,5,3,2,6,6,6,6,6,6,6,6,6,6,6,
	3,2,3,3,3,2,2,2,6,6,6,6,6,6,6,6,
	6,6,2,3,5,3,5,6,6,6,2,3,5,3,5,5,
	6,6,6,6,6,4,4,3,6,6,2,3,5,5,3,3,
	6,6,2,3,5,3,5,5,6,6,2,3,5,4,5,5,
	6,6,2,4,3,3,2,2,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,2,2,3,4,6,
	6,6,2,3,3,3,5,6,2,6,6,6,2,4,5,6,
	5,3,3,3,2,6,6,6,6,6,6,6,6,6,6,6,
	0,4,3,3,2,3,2,6,2,2,2,0,3,3,2,6,
	2,0,2,3,3,6,6,6,2,6,3,2,6,6,6,6,
	3,2,6,6,6,6,3,2,6,6,0,2,5,3,2,3,
	2,2,3,4,3,2,3,0,0,2,3,3,5,3,6,6,
	6,3,2,0,6,6,6,3,2,6,6,6,3,2,0,2,
	6,6,6,2,3,3,5,5,6,6,6,6,6,6,6,6,
	0,4,3,3,2,3,2,6,2,4,3,3,2,0,0,6,
	2,6,0,2,6,6,6,6,3,2,6,6,6,6,3,2,
	6,6,2,2,3,5,5,6,6,5,6,6,6,6,6,6,
	6,6,5,3,3,2,0,3,2,2,2,0,4,3,3,0,
	6,3,3,0,6,6,6,2,2,6,6,6,3,0,3,2,
	2,0,3,3,5,3,5,6,2,3,2,0,6,6,6,6,
	2,4,3,3,2,3,2,6,2,2,0,3,3,2,0,6,
	6,6,3,2,6,6,6,6,6,6,3,0,6,6,3,2,
	6,6,0,3,3,4,6,6,0,0,6,6,6,6,6,6,
	6,4,3,3,2,3,4,6,2,3,3,4,5,5,6,6,
	6,6,6,6,6,6,6,6,6,6,3,6,6,6,6,6,
	2,6,0,2,3,2,2,3,2,3,2,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,6,5,5,3,3,
	2,2,2,2,3,2,2,3,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,3,5,3,3,2,2,0,
	6,6,6,5,6,6,6,6,3,3,2,0,0,3,3,2,
	6,6,2,0,6,3,2,6,6,6,2,0,6,6,6,6,
	6,6,6,3,2,0,2,6,3,5,3,3,2,3,3,3,
	5,5,3,3,3,0,2,6,2,3,2,0,2,3,2,2,
	6,6,6,6,3,4,5,6,2,2,6,2,5,3,2,6,
	6,6,6,6,6,6,6,6,6,6,2,0,0,6,2,3,
	6,6,6,6,0,6,6,6,2,0,3,3,5,5,3,3,
	3,3,3,4,3,3,3,2,0,3,3,3,4,5,5,6,
	6,6,6,6,6,6,6,6,6,2,2,2,3,0,0,0,
	3,3,3,4,3,3,3,3,3,3,3,5,5,4,3,6,
	6,6,6,3,2,6,6,6,4,3,6,2,2,6,6,6,
	2,2,3,3,4,5,3,3,2,2,2,3,3,5,3,3,
	3,5,5,5,5,5,5,5,6,2,6,6,6,3,3,0,
	0,6,6,6,3,3,6,6,6,6,6,2,2,5,3,3,
	2,2,2,2,0,0,2,4,3,2,3,3,3,3,3,3,
	5,5,5,5,5,5,5,5,3,6,6,6,3,6,6,6,
	6,6,6,3,0,6,3,3,2,0,3,3,4,3,3,3,
	3,2,2,0,0,3,5,3,3,2,3,3,3,3,3,3,
	3,3,3,3,5,5,5,5,6,6,3,2,0,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,6,6,2,0,6,
	2,2,3,3,5,3,5,3,4,3,3,2,4,3,3,2,
	0,0,0,0,2,2,2,2,6,3,2,2,6,6,6,6,
	6,6,6,6,3,2,6,6,6,6,6,6,6,6,6,6,
	2,2,3,4,5,5,3,3,3,3,2,5,3,3,3,2,
	0,2,3,3,3,3,6,6,6,6,5,3,6,6,6,2,
	6,6,6,2,0,6,0,6,5,5,1,3,3,6,6,6,
	6,4,3,3,2,3,4,5,6,6,6,6,6,6,6,6,
	6,6,3,2,6,6,6,6,6,6,0,0,5,3,2,0,
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	6,6,5,3,3,2,3,5,6,6,2,3,5,4,6,6,
	6,5,3,2,3,3,3,5,6,6,6,6,6,6,5,3,
	2,0,2,3,4,5,5,6,6,2,5,3,3,3,5,6,
	6,6,6,2,5,3,3,5,6,4,5,3,0,3,4,5,
	6,5,3,2,0,3,5,6,6,6,6,5,3,3,2,3,
	6,2,3,6,6,6,6,6,6,6,6,2,3,5,6,6,
	6,4,2,6,6,6,2,3,3,4,5,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,2,3,5,6,6,6,
	6,3,2,3,3,3,3,5,6,4,5,3,2,6,6,6,
	6,6,6,2,2,2,5,3,6,6,6,6,6,6,5,6,
	6,6,6,6,6,6,6,6,6,6,6,2,2,6,6,6,
	6,6,5,3,2,0,2,3,6,6,6,6,2,3,5,5,
	2,4,3,3,2,3,2,3,2,2,2,5,3,3,3,6,
	2,0,0,6,6,6,6,6,6,6,6,6,6,6,3,3,
	6,6,6,3,3,2,2,2,6,6,2,0,0,6,6,4,
	6,6,6,6,6,6,3,0,6,6,6,6,6,6,6,6,
	2,0,0,6,6,6,6,6,6,6,6,6,6,2,3,0,
	6,5,4,3,3,4,6,6,3,3,3,3,3,2,2,2,
	0,0,6,6,6,6,6,6,6,6,6,6,2,0,6,6,
	5,4,3,2,6,6,6,6,6,6,6,6,6,6,6,6,
	3,3,3,2,2,6,6,6,6,6,6,6,6,6,0,0,
	6,6,2,3,5,4,6,6,6,6,2,3,4,5,3,3,
	6,6,6,6,6,5,3,3,6,6,2,3,4,3,3,3,
	6,6,2,3,4,3,5,3,6,6,2,3,4,5,3,3,
	6,6,4,3,3,2,2,4,6,4,5,4,6,6,6,6,
	2,3,2,3,5,6,6,6,6,6,6,2,3,3,5,6,
	6,6,2,3,3,2,5,6,6,6,6,6,2,3,5,6,
	3,3,3,2,6,6,6,6,6,6,6,6,6,6,6,6,
	6,5,3,2,2,3,2,6,0,0,0,0,5,3,3,6,
	6,6,2,2,3,6,6,6,6,3,3,6,6,6,6,6,
	3,0,6,6,6,6,2,0,6,6,2,0,2,3,3,3,
	2,0,2,5,3,3,2,6,6,2,2,3,3,6,6,6,
	6,3,2,6,6,6,6,3,0,6,6,2,2,0,2,0,
	6,6,6,6,6,2,3,3,6,5,5,5,5,6,6,6,
	6,2,3,3,3,2,2,6,0,5,3,3,2,0,0,6,
	6,0,2,6,6,6,6,6,2,0,6,6,6,6,2,0,
	6,6,6,6,2,3,3,5,6,3,5,5,5,6,6,6,
	6,6,4,3,2,0,2,3,2,0,0,0,5,3,3,6,
	6,3,2,6,6,6,6,3,0,6,6,2,0,3,2,0,
	6,6,2,3,3,4,6,6,0,2,0,0,6,6,6,6,
	6,5,3,2,2,3,2,6,0,0,0,2,5,3,2,6,
	6,3,3,6,6,6,6,6,6,3,2,0,6,6,2,0,
	6,6,6,2,3,6,6,6,6,6,6,6,6,6,6,6,
	6,5,3,2,2,3,3,6,0,2,2,3,3,3,5,5,
	5,5,5,5,5,6,6,6,6,3,3,6,6,6,6,6,
	6,6,5,5,3,3,2,2,2,2,5,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,5,5,3,3,3,3,
	0,0,5,5,3,3,2,2,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,5,5,5,3,3,2,0,
	6,6,6,5,6,6,6,6,3,3,2,2,0,2,3,0,
	6,3,0,6,6,6,0,6,6,3,0,6,6,6,6,6,
	6,6,6,2,3,0,0,6,5,3,5,3,2,2,3,3,
	5,3,3,3,2,2,0,6,0,2,0,0,2,5,3,0,
	6,6,6,6,2,2,5,6,0,6,6,2,3,2,0,6,
	6,6,6,6,6,6,6,6,6,6,3,2,0,6,3,2,
	6,6,6,6,6,6,6,6,6,6,2,3,3,5,3,3,
	2,2,5,3,3,3,0,0,2,2,3,3,3,3,3,6,
	6,6,6,6,6,6,6,6,6,6,6,6,3,2,6,6,
	2,3,5,3,3,3,3,2,2,2,3,3,3,2,2,6,
	6,6,6,6,5,6,6,6,3,0,6,6,6,6,6,6,
	6,6,2,2,3,5,3,3,3,4,0,2,5,3,3,3,
	3,3,3,3,3,3,3,3,6,6,6,6,6,6,0,0,
	6,6,6,6,6,3,6,6,6,6,6,0,5,3,3,3,
	3,0,2,0,0,0,0,0,3,3,2,2,2,2,2,2,
	2,2,2,2,6,6,6,6,6,6,6,6,6,6,6,6,
	6,6,6,3,2,6,6,6,6,6,2,3,5,3,3,2,
	3,2,0,0,6,6,5,3,3,2,0,2,2,2,2,6,
	6,6,6,6,6,6,6,6,6,3,2,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,0,0,
	6,6,2,3,3,5,3,3,3,3,3,2,4,3,3,3,
	2,0,0,6,6,6,6,6,3,3,6,6,6,6,6,6,
	6,6,6,6,3,2,6,6,6,6,6,6,6,6,6,6,
	6,6,2,3,5,3,3,3,2,2,4,3,3,3,2,0,
	0,0,0,2,2,2,6,6,6,6,3,2,6,6,6,6,
	6,6,6,6,0,0,6,6,3,3,3,3,2,6,6,6,
	6,5,3,2,2,3,3,4,6,6,6,6,6,6,5,6,
	6,3,3,6,6,6,6,6,6,6,6,5,3,2,2,0,
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,2,3,6,6,6,
	6,5,2,0,3,2,3,4,6,6,6,6,6,5,3,2,
	2,0,2,2,3,3,3,5,6,2,5,3,3,3,5,6,
	6,6,6,2,5,3,3,2,6,6,3,3,0,3,3,5,
	6,6,3,3,2,2,3,5,6,6,6,3,3,2,0,3,
	6,6,3,6,6,6,6,6,6,6,6,6,6,3,5,6,
	6,2,0,6,6,6,6,2,2,3,3,5,5,5,6,6,
	6,6,6,6,6,6,6,6,6,6,3,3,4,5,6,6,
	6,2,2,3,3,3,5,3,6,6,4,3,2,6,6,6,
	6,6,6,6,6,2,5,3,6,6,6,6,6,6,3,5,
	6,6,6,6,6,6,6,6,6,6,6,6,3,6,6,6,
	6,6,6,3,3,2,6,2,6,6,6,6,6,6,6,6,
	6,2,3,3,3,3,2,2,2,0,0,5,3,3,3,6,
	0,0,6,6,6,6,6,6,6,6,6,6,6,6,6,3,
	6,6,5,3,3,2,0,0,6,6,0,0,6,6,6,6,
	6,6,6,6,6,6,3,2,6,6,6,6,6,6,6,6,
	0,0,6,6,6,6,6,6,6,6,6,6,6,6,3,0,
	6,5,3,3,2,2,6,6,3,3,3,2,2,6,6,6,
	2,6,6,6,6,6,6,6,6,6,6,6,0,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	3,3,2,6,6,6,6,6,6,6,6,6,6,6,2,6,
	6,6,6,2,3,6,6,6,6,6,6,2,3,5,3,3,
	6,6,6,0,5,3,3,3,6,6,2,3,5,3,3,2,
	6,6,2,2,3,5,3,3,6,6,6,2,5,3,3,3,
	5,5,4,3,2,0,3,5,5,3,3,3,6,6,6,6,
	2,3,3,3,4,6,6,6,6,6,6,2,3,3,5,6,
	6,6,6,2,3,2,4,6,6,6,6,6,6,2,4,5,
	6,3,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	6,3,3,3,2,2,2,6,6,6,6,6,6,6,6,6,
	6,6,6,2,2,6,6,6,0,2,0,6,6,6,2,6,
	0,6,6,6,6,2,0,6,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,2,2,6,6,6,6,
	0,2,6,6,6,6,0,2,6,6,6,6,0,2,0,6,
	6,6,6,6,6,6,6,2,6,3,2,2,3,5,5,5,
	6,0,2,4,3,0,2,6,6,6,6,6,6,6,6,6,
	0,2,0,6,6,6,2,6,0,6,6,6,6,0,0,6,
	6,6,6,6,6,6,2,3,6,6,2,2,3,5,5,5,
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	0,2,6,6,6,6,0,2,6,6,6,6,2,2,0,6,
	6,6,6,2,3,6,6,6,0,0,6,6,6,6,6,6,
	6,3,3,3,2,2,2,6,6,6,6,6,6,6,6,6,
	0,2,0,6,6,6,6,6,0,2,0,6,6,2,0,6,
	6,6,6,2,6,6,6,6,6,6,6,6,6,6,6,6,
	6,3,3,3,2,3,3,5,0,0,2,2,2,2,2,2,
	2,3,2,3,3,5,5,5,0,2,0,6,6,6,6,6,
	6,2,2,0,2,2,2,0,0,2,2,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,0,2,0,0,2,2,0,
	0,2,2,0,2,2,2,0,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,5,3,5,5,3,3,0,
	6,6,6,5,6,6,6,6,4,3,2,2,0,2,0,6,
	0,2,0,6,6,6,6,0,0,2,6,6,6,6,6,6,
	6,6,6,0,0,0,6,6,3,5,3,2,2,6,6,6,
	5,4,3,2,2,0,2,5,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,0,2,0,6,6,
	6,6,6,6,6,6,6,6,6,0,2,0,6,0,2,0,
	6,6,6,6,6,6,6,6,6,6,6,2,3,5,3,3,
	3,2,5,3,3,2,0,0,0,2,2,2,2,2,6,6,
	6,6,6,6,6,6,6,6,6,6,6,6,2,0,6,6,
	3,4,5,3,3,3,2,0,0,2,2,2,6,6,6,6,
	6,6,6,3,2,6,6,6,6,6,6,6,6,6,6,6,
	6,6,6,2,2,3,3,3,3,3,0,2,5,3,3,3,
	3,2,2,2,2,2,2,6,6,6,6,6,6,6,6,6,
	6,6,6,6,3,2,6,6,6,6,0,2,5,3,2,0,
	0,0,0,6,6,6,6,0,3,3,0,2,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	6,6,3,2,6,6,6,6,6,6,6,3,5,3,3,2,
	2,0,0,6,6,6,6,5,3,2,0,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	6,6,6,2,5,3,3,3,2,3,2,2,0,5,3,3,
	3,2,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	6,6,6,3,2,6,6,6,6,6,6,6,6,6,6,6,
	6,6,6,2,5,3,3,3,2,2,4,3,3,2,2,0,
	0,6,6,6,6,6,6,6,6,3,2,6,6,6,6,6,
	6,6,6,6,6,6,6,6,3,2,2,2,2,6,6,6,
	6,4,3,3,2,2,3,3,6,6,6,6,6,6,5,5,
	0,2,0,6,6,6,6,6,6,6,2,2,0,2,0,0,
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,2,6,6,6,6,
	6,3,2,0,2,3,3,4,6,6,6,6,2,2,0,2,
	0,0,0,2,2,2,2,2,6,3,4,3,2,3,5,6,
	6,6,6,6,5,3,3,2,6,6,2,3,0,2,3,5,
	6,6,6,3,2,0,3,4,6,6,6,3,3,3,0,2,
	6,6,2,6,6,6,6,6,6,6,6,6,6,6,3,5,
	2,0,6,6,6,6,6,6,6,2,2,2,2,2,3,5,
	6,6,6,6,6,6,6,6,6,6,3,3,3,5,6,6,
	6,2,2,3,2,3,4,3,6,6,5,2,0,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,3,5,
	6,6,6,6,6,6,6,6,6,6,6,6,2,6,6,6,
	6,6,6,6,3,2,6,6,6,6,6,6,6,6,6,6,
	6,0,2,4,3,2,2,0,0,0,0,4,3,3,2,6,
	6,2,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	6,6,4,3,3,0,0,6,6,6,0,6,6,6,6,6,
	6,6,6,6,6,6,3,3,6,6,6,6,6,6,6,6,
	6,2,6,6,6,6,6,6,6,6,6,6,6,6,2,6,
	5,4,3,2,2,0,6,6,4,3,2,6,6,6,6,6,
	3,2,6,6,6,6,6,6,6,6,6,6,0,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	4,2,6,6,6,6,6,6,6,6,6,6,6,6,3,2,
	6,6,6,6,2,6,6,6,6,6,6,2,2,5,3,3,
	6,6,0,2,5,3,2,0,6,6,6,3,5,3,3,2,
	6,6,6,2,5,3,3,3,6,6,6,2,5,3,3,3,
	3,3,3,3,0,0,3,4,2,2,2,2,6,6,6,6,
	6,3,4,3,5,6,6,6,6,6,6,0,4,4,5,6,
	6,6,6,3,3,2,4,4,6,6,6,6,6,0,3,5
	};
