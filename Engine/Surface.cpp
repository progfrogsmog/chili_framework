#include "Surface.h"
#include "ChiliWin.h"
#include <cassert>
#include <fstream>

Surface::Surface(const Surface& src)
	:
	width(src.width),
	height(src.height)
{
	pPixel = new Color[width * height];
	for (int i = 0; i < width * height; i++)
	{
		pPixel[i] = src.pPixel[i];
	}
}

Surface::Surface(int width, int height)
	:
	width(width),
	height(height)
{
	pPixel = new Color[width * height];
}

Surface::Surface(const std::string filename)
{
	/* OPEN FILE TO READ */
	std::ifstream file(filename, std::ios::binary);
	assert(file);

	/* READ INFO FROM FILE */
	BITMAPFILEHEADER bmFileHeader;
	file.read(reinterpret_cast<char*>(&bmFileHeader), sizeof(bmFileHeader));

	BITMAPINFOHEADER bmInfoHeader;
	file.read(reinterpret_cast<char*>(&bmInfoHeader), sizeof(bmInfoHeader));

	assert(bmInfoHeader.biBitCount == 24 || bmInfoHeader.biBitCount == 32);
	assert(bmInfoHeader.biCompression == BI_RGB);

	/* GET DIMENSIONS OPERATION */
	int yStart;
	int yEnd;
	int dy;
	if (bmInfoHeader.biHeight < 0)
	{
		height = -bmInfoHeader.biHeight;
		yStart = 0;
		yEnd = height;
		dy = 1;
	}
	else
	{
		height = bmInfoHeader.biHeight;
		yStart = height - 1;
		yEnd = -1;
		dy = -1;
	}
	width = bmInfoHeader.biWidth;
	pPixel = new Color[width * height];

	/* DRAW BITMAP DEPENDENT ON FORMAT */
	file.seekg(bmFileHeader.bfOffBits);
	const int padding = (4 - (3 * width % 4)) % 4; //FOR PADDING SPACE AT THE END OF SINGLE ROW
	bool is32 = bmInfoHeader.biBitCount == 32;

	for (int y = yStart; y != yEnd; y += dy)//IMPORTANT: MUST BE != BECAUSE CAN BE < or > dependent on height
	{
		for (int x = 0; x < width; x++)
		{
			PutPixel(x, y, Color(file.get(), file.get(), file.get()));
			if (is32)
			{
				file.seekg(1,std::ios::cur);//file.get() works also
			}
		}
		if (!is32)
		{
			file.seekg(padding, std::ios::cur);
		}
	}
}

Surface& Surface::operator=(const Surface& src)
{
	width = src.width;
	height = src.height;

	delete pPixel;
	pPixel = new Color[width * height];

	for (int i = 0; i < width * height; i++)
	{
		pPixel[i] = src.pPixel[i];
	}
	return *this;
}

Surface::~Surface()
{
	delete pPixel;
	pPixel = nullptr;
}

int Surface::GetWidth() const
{
	return width;
}

int Surface::GetHeight() const
{
	return height;
}

void Surface::PutPixel(int x, int y, Color& color)
{
	pPixel[x + y * width] = color;
}

Color Surface::GetPixel(int x, int y) const
{
	return pPixel[x + y * width];
}

RectI Surface::GetRect() const
{
	return {0,width,0,height};
}
