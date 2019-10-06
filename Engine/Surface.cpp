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
	std::ifstream file("dib.bmp", std::ios::binary);
	assert(file);

	BITMAPFILEHEADER bmFileHeader;
	file.read(reinterpret_cast<char*>(&bmFileHeader), sizeof(bmFileHeader));

	BITMAPINFOHEADER bmInfoHeader;
	file.read(reinterpret_cast<char*>(&bmInfoHeader), sizeof(bmInfoHeader));

	assert(bmInfoHeader.biBitCount == 24);
	assert(bmInfoHeader.biCompression == BI_RGB);

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
	file.seekg(bmFileHeader.bfOffBits);

	const int padding = (4 - (3 * width % 4)) % 4; //FOR PADDING SPACE AT THE END OF SINGLE ROWlkj

	for (int y = yStart - 1; y >= 0; y--)
	{
		for (int x = 0; x < width; x++)
		{
			PutPixel(x, y, Color(file.get(), file.get(), file.get()));
		}
		file.seekg(padding, std::ios::cur);
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
