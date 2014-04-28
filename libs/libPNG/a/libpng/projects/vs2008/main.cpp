

#include "LibPNG.h"

using namespace Libs;

int main(int argc, char **argv)
{
	ReadPNG *image = createReadPNG();
	image->openfile("H:\\libraries\\utils\\cut.png");
	PNGImage img;
	image->getinfo(&img);
	img.buffer = new unsigned char[img.rowbytes * img.height];
	image->getinfo(&img);
	image->close();
	image->destroy();
	return 0;
}

