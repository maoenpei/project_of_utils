

#include <windows.h>
#include "ft2build.h"
#include "freetype.h"
#pragma warning(disable:4996)

#include "LibFreeType.h"

int gettimeofday(struct timeval * val, struct timezone *)
{
    if (val)
    {
        LARGE_INTEGER liTime, liFreq;
        QueryPerformanceFrequency( &liFreq );
        QueryPerformanceCounter( &liTime );
        val->tv_sec     = (long)( liTime.QuadPart / liFreq.QuadPart );
        val->tv_usec    = (long)( liTime.QuadPart * 1000000.0 / liFreq.QuadPart - val->tv_sec * 1000000.0 );
    }
    return 0;
}

static int getMilliTime()
{
	timeval val;
	gettimeofday(&val, NULL);
	return (int)val.tv_sec * 1000000 + val.tv_usec;
}

int main(int argc, char **argv)
{
#define FONT_FILE		"FZSTK.TTF"
#define FONT_SIZE		25
	Libs::FreeType * ft = createFreeType();
	ft->LoadLibMemory(FONT_FILE);
	Libs::FontBitmap bmp;
	bmp.width = FONT_SIZE;
	bmp.height = FONT_SIZE;
	bmp.buffer = new unsigned char[FONT_SIZE * FONT_SIZE];
	ft->getFontMetrics(L'Œ“', FONT_SIZE, NULL);
	ft->RenderFont(L'Œ“', FONT_SIZE, &bmp);

	return 0;
}

