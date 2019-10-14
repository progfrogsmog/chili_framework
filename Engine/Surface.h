#pragma once

#include "Colors.h"
#include "RectI.h"
#include <string>

class Surface
{
public:
	Surface(const Surface& src);
	Surface(int width, int height);
	Surface(const std::string filename);
	Surface& operator=(const Surface& src);
	~Surface();
	int GetWidth() const;
	int GetHeight() const;
	void PutPixel(int x, int y, Color& color);
	Color GetPixel(int x, int y) const;
	RectI GetRect() const;
private:
	Color* pPixel = nullptr;
	int width;
	int height;
};