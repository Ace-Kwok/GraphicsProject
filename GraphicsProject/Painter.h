#pragma once
#include <vector>
#include "Texture.h"
#include "mesh.h"

struct Pixel
{
	int x;
	int y;
	double z; //…Ó∂»
	double r, g, b;
	Pixel(){}
	Pixel(int xx, int yy, double zz = 0, double rr = 255, double gg = 255, double bb = 255)
	{
		x = xx;
		y = yy;
		z = zz;
		r = rr;
		g = gg;
		b = bb;
	}
};

class Painter
{
public:
	int height, width;
	std::vector<Pixel> mPixels;
	Texture pic;

	void Clear();
	void SetTexture(const char*);
	void AddPixel(Pixel p);
	void DDALine(Vertex A, Vertex B);
	void myGouraudFill(Vertex A, Vertex B, Vertex C);
	void myGouraudMapping(Vertex A, Vertex B, Vertex C, const Camera& cam, const AmbtLight& ambl, const DirectLight& dirl);

	void CutTriangle(Vertex A, Vertex B, Vertex C, const Camera& cam, const AmbtLight& ambl, const DirectLight& dirl);

	Painter() {}
	Painter(int w, int h) : height(h), width(w) {}
};

