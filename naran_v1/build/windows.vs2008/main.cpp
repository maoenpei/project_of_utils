
#include "Naran.h"
#include "png.h"
#include "jpeglib.h"

#include <iostream>
#include <math.h>

using namespace Naran;

int main(int argc, char **argv)
{
	grab(Image) img = ImageLoader::sharedLoader(".jpg")->decodeImage("game_loading4.jpg");
	
	return 0;
}


