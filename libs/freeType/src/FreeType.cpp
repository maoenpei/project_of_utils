

#include "FreeType.h"

#include "../a/freetype-2.5.3/include/ft2build.h"
#include "../a/freetype-2.5.3/include/freetype.h"

#define SIZE2PX(size)		((size)>>6)

class FreeTypeImpl : public Libs::FreeType
{
public:
	LIB_IMPL_DESTROY();

	FT_Library m_ftlib;
	FreeTypeImpl() 
		: m_ftlib(0), m_ftface(0), m_filebuf(0)
		, m_last_fontsize(0), m_last_unicode(0), m_font_rendered(false)
	{
		FT_Init_FreeType(&m_ftlib);
	}
	~FreeTypeImpl(){
		if (m_ftface){
			FreeLib();
		}
		FT_Done_FreeType(m_ftlib);
	}

	FT_Face m_ftface;
	unsigned char *m_filebuf;
	virtual void LoadLibFile(const char *pathname)
	{
		if (m_ftlib == 0 || m_ftface){
			return;
		}
		FT_New_Face(m_ftlib, pathname, 0, &m_ftface);
	}
	virtual void LoadLibMemory(const char *pathname)
	{
		if (m_ftlib == 0 || m_ftface){
			return;
		}
		FILE *f = fopen(pathname, "rb");
		if (f == 0){
			return;
		}
		fseek(f, 0, SEEK_END);
		long fsize = ftell(f);
		fseek(f, 0, SEEK_SET);
		m_filebuf = new unsigned char[fsize];
		fread(m_filebuf, 1, fsize, f);
		fclose(f);
		FT_New_Memory_Face(m_ftlib, m_filebuf, fsize, 0, &m_ftface);
	}
	virtual void FreeLib()
	{
		if (m_ftface == 0){
			return ;
		}
		FT_Done_Face(m_ftface);
		m_ftface = 0;
		if (m_filebuf){
			delete [] m_filebuf;
			m_filebuf = 0;
		}
	}

	unsigned long m_last_unicode;
	int m_last_fontsize;
	bool m_font_rendered;
	virtual void getFontMetrics(unsigned long unicode, int fontSize, Libs::FontMetrics *metrics)
	{
		if (m_ftface == 0){
			return ;
		}
		// load glyph
		if (m_last_unicode != unicode || m_last_fontsize != fontSize){
			m_last_unicode = unicode;
			m_last_fontsize = fontSize;
			m_font_rendered = false;
			FT_Set_Char_Size(m_ftface, 0, fontSize * 64, 72, 72);
			FT_Load_Char(m_ftface, unicode, FT_LOAD_NO_BITMAP);
		}
		// compute metrics
		if (metrics){
			FT_Glyph_Metrics *fontMetrics = &m_ftface->glyph->metrics;
			metrics->fontSize = fontSize;
			metrics->wSize = SIZE2PX(fontMetrics->width);
			metrics->hSize = SIZE2PX(fontMetrics->height);
			metrics->hAdvance = SIZE2PX(fontMetrics->vertAdvance);
			metrics->hBaseSize = SIZE2PX(fontMetrics->height - fontMetrics->horiBearingY);
			metrics->hBaseAdvance = SIZE2PX(fontMetrics->vertAdvance - fontMetrics->vertBearingY - fontMetrics->horiBearingY);
		}
	}
	virtual void RenderFont(unsigned long unicode, int fontSize, Libs::FontBitmap *bmp)
	{
		if (m_ftface == 0){
			return ;
		}
		// load glyph
		if (m_last_unicode != unicode || m_last_fontsize != fontSize){
			m_last_unicode = unicode;
			m_last_fontsize = fontSize;
			m_font_rendered = false;
			FT_Set_Char_Size(m_ftface, 0, fontSize * 64, 72, 72);
			FT_Load_Char(m_ftface, unicode, FT_LOAD_NO_BITMAP);
		}
		// execute render
		if (! m_font_rendered){
			m_font_rendered = true;
			FT_Render_Glyph(m_ftface->glyph, FT_RENDER_MODE_NORMAL);
		}
		// copy bitmap
		if (bmp){
			int glyph_width = m_ftface->glyph->bitmap.width;
			int glyph_height = m_ftface->glyph->bitmap.rows;
			unsigned char *from = m_ftface->glyph->bitmap.buffer;
			unsigned char *to = bmp->buffer;
			int min_width = (glyph_width <= bmp->width ? glyph_width : bmp->width);
			int min_height = (glyph_height <= bmp->height ? glyph_height : bmp->height);
			for (int i = 0; i<min_height; i++){
				memcpy(to, from, min_width);
				from += glyph_width;
				to += bmp->width;
			}
		}
	}
};

Libs::FreeType * createFreeType()
{
	return new FreeTypeImpl();
}
