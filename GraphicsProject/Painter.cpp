#include "Painter.h"
#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define MAX(x,y) ((x) > (y) ? (x) : (y))

void Painter::Clear()
{
	std::vector<Pixel>().swap(mPixels);
}

void Painter::SetTexture(const char* path)
{
	pic.SetTexture(path);
}


void Painter::AddPixel(Pixel p)
{
	mPixels.push_back(p);
}

void Painter::DDALine(Vertex A, Vertex B)
{
	double deltaX, deltaY, deltaZ, deltaR, deltaG, deltaB;
	double x, y, z;
	int dx, dy;
	double dr, dg, db;
	double dz;
	int steps;
	double r, g, b;

	dx = (int)B.pos.x - (int)A.pos.x;
	dy = (int)B.pos.y - (int)A.pos.y;
	dz = B.pos.z - A.pos.z;
	dr = B.r - A.r;
	dg = B.g - A.g;
	db = B.b - A.b;
	if (abs(dx) > abs(dy))
	{
		steps = abs(dx);
	}
	else
	{
		steps = abs(dy);
	}
	deltaX = ((double)dx) / ((double)steps);
	deltaY = ((double)dy) / ((double)steps);
	deltaZ = dz / (double)steps;
	deltaR = dr / (double)steps;
	deltaG = dg / (double)steps;
	deltaB = db / (double)steps;
	x = (int)A.pos.x;
	y = (int)A.pos.y;
	z = (int)A.pos.z;
	r = A.r;
	g = A.g;
	b = A.b;
	for (int k = 1; k <= steps; ++k)
	{
		x += deltaX;
		y += deltaY;
		z += deltaZ;
		r += deltaR;
		g += deltaG;
		b += deltaB;
		mPixels.push_back(Pixel(x, y ,z , r, g, b));
	}
}

void Painter::myGouraudFill(Vertex A, Vertex B, Vertex C)
{


	int minX = MAX(0, MIN(A.pos.x, MIN(B.pos.x, C.pos.x)));
	int minY = MAX(0, MIN(A.pos.y, MIN(B.pos.y, C.pos.y)));
	int MaxX = MIN(width, MAX(A.pos.x, MAX(B.pos.x, C.pos.x)));
	int MaxY = MIN(height, MAX(A.pos.y, MAX(B.pos.y, C.pos.y)));

	int ux = B.pos.x - A.pos.x, uy = B.pos.y - A.pos.y, vx = C.pos.x - A.pos.x, vy = C.pos.y - A.pos.y;
	double p = ux * vy - vx * uy;

	for (int i = minY; i <= MaxY; i++)
	{
		for (int j = minX; j <= MaxX; j++)
		{
			int px = j, py = i;
			double s = 1.0 / p * (A.pos.y * C.pos.x - A.pos.x * C.pos.y + (C.pos.y - A.pos.y) * px + (A.pos.x - C.pos.x) * py);
			double t = 1.0 / p * (A.pos.x * B.pos.y - A.pos.y * B.pos.x + (A.pos.y - B.pos.y) * px + (B.pos.x - A.pos.x) * py);
			double n = 1 - s - t;
			if (s > 1e-15 && t > 1e-15 && n > 1e-15)
			{
				double z = (1 / A.pos.z) * n + (1 / B.pos.z) * s + (1 / C.pos.z) * t;
				double r = A.r * n / A.pos.z + B.r * s / B.pos.z + C.r * t / C.pos.z;
				double g = A.g * n / A.pos.z + B.g * s / B.pos.z + C.g * t / C.pos.z;
				double b = A.b * n / A.pos.z + B.b * s / B.pos.z + C.b * t / C.pos.z;
				mPixels.push_back(Pixel(j, i, 1/z, r/z, g/z, b/z));
			}
		}
	}
}

void Painter::myGouraudMapping(Vertex A, Vertex B, Vertex C, const Camera& cam, const AmbtLight& ambl, const DirectLight& dirl)
{
	
	A.setColor(cam, ambl, dirl);
	B.setColor(cam, ambl, dirl);
	C.setColor(cam, ambl, dirl);


	int minX = MAX(0, MIN(A.pos.x, MIN(B.pos.x, C.pos.x)));
	int minY = MAX(0, MIN(A.pos.y, MIN(B.pos.y, C.pos.y)));
	int MaxX = MIN(width, MAX(A.pos.x, MAX(B.pos.x, C.pos.x)));
	int MaxY = MIN(height, MAX(A.pos.y, MAX(B.pos.y, C.pos.y)));

	int ux = B.pos.x - A.pos.x, uy = B.pos.y - A.pos.y, vx = C.pos.x - A.pos.x, vy = C.pos.y - A.pos.y;
	double p = ux * vy - vx * uy;

	for (int i = minY; i <= MaxY; i++)
	{
		for (int j = minX; j <= MaxX; j++)
		{
			int px = j, py = i;
			double s = 1.0 / p * (A.pos.y * C.pos.x - A.pos.x * C.pos.y + (C.pos.y - A.pos.y) * px + (A.pos.x - C.pos.x) * py);
			double t = 1.0 / p * (A.pos.x * B.pos.y - A.pos.y * B.pos.x + (A.pos.y - B.pos.y) * px + (B.pos.x - A.pos.x) * py);
			double n = 1.0 - s - t;
			if (s > 1e-15 && t > 1e-15 && n > 1e-15)
			//if ((s > 1e-15 || abs(s) < 1e-15) && (t > 1e-15 || abs(t) < 1e-15) && (n > 1e-15 || abs(n) < 1e-15))
			{
				double z = (1 / A.pos.z) * n + (1 / B.pos.z) * s + (1 / C.pos.z) * t;
				double u = (A.u / A.pos.z) * n + (B.u / B.pos.z) * s + (C.u / C.pos.z) * t;
				double v = (A.v / A.pos.z) * n + (B.v / B.pos.z) * s + (C.v / C.pos.z) * t;
				z = 1 / z;
				if (z < cam.ZNear) continue;
				u = u * z;
				v = v * z;
				if (u > 1 || v > 1 || u < 0 || v < 0) continue;
				double r = A.r * n + B.r * s + C.r * t;
				r = pic.mTexture[(int)((pic.mTextHeight - 1) * v)][(int)((pic.mTextWidth - 1) * u)].R * r / 255.0;
				double g = A.g * n + B.g * s + C.g * t;
				g = pic.mTexture[(int)((pic.mTextHeight - 1) * v)][(int)((pic.mTextWidth - 1) * u)].G * g / 255.0;
				double b = A.b * n + B.b * s + C.b * t;
				b = pic.mTexture[(int)((pic.mTextHeight - 1) * v)][(int)((pic.mTextWidth - 1) * u)].B * b / 255.0;
				mPixels.push_back(Pixel(j, i, z, r, g, b));
			}
		}
	}

}

void Painter::CutTriangle(Vertex A, Vertex B, Vertex C, const Camera& cam, const AmbtLight& ambl, const DirectLight& dirl)
{
	if ((B.pos.x - A.pos.x) * (C.pos.y - A.pos.y) - (B.pos.y - A.pos.y) * (C.pos.x - A.pos.x) > 0) return; //±³ÃæÌÞ³ý


	Vertex m;


	if (B.pos.z < A.pos.z)
	{
		m = A;
		A = B;
		B = m;
	}

	if (C.pos.z < B.pos.z)
	{
		m = B;
		B = C;
		C = m;
	}
	

	double t;
	if (A.pos.z > cam.ZNear) myGouraudMapping(A, B, C, cam, ambl, dirl);
	else if (B.pos.z > cam.ZNear && A.pos.z < cam.ZNear)
	{
		Vertex newb, newc;
		t = (1 / cam.ZNear - 1 / A.pos.z) / (1 / B.pos.z - 1 / A.pos.z);
		newb = A;
		newb.pos = (A.pos / A.pos.z + (B.pos / B.pos.z - A.pos / A.pos.z) * t) * cam.ZNear;
		newb.u = (A.u / A.pos.z + t * (B.u / B.pos.z - A.u / A.pos.z) * t) * cam.ZNear;
		newb.v = (A.v / A.pos.z + t * (B.v / B.pos.z - A.v / A.pos.z) * t) * cam.ZNear;

		t = (1 / cam.ZNear - 1 / A.pos.z) / (1 / C.pos.z - 1 / A.pos.z);
		newc = A;
		newc.pos = (A.pos / A.pos.z + (C.pos / C.pos.z - A.pos / A.pos.z) * t) * cam.ZNear;
		newc.u = (A.u / A.pos.z + t * (C.u / C.pos.z - A.u / A.pos.z) * t) * cam.ZNear;
		newc.v = (A.v / A.pos.z + t * (C.v / C.pos.z - A.v / A.pos.z) * t) * cam.ZNear;

		myGouraudMapping(C, newb, newc, cam, ambl, dirl);
		myGouraudMapping(C, newb, B, cam, ambl, dirl);
	}
	else if (C.pos.z > cam.ZNear && B.pos.z < cam.ZNear)
	{
		Vertex newb, newa;
		t = (1 / cam.ZNear - 1 / C.pos.z) / (1 / B.pos.z - 1 / C.pos.z);
		newb = C;
		newb.pos = (C.pos / C.pos.z + (B.pos / B.pos.z - C.pos / C.pos.z) * t) * cam.ZNear;
		newb.u = (C.u / C.pos.z + t * (B.u / B.pos.z - C.u / C.pos.z) * t) * cam.ZNear;
		newb.v = (C.v / C.pos.z + t * (B.v / B.pos.z - C.v / C.pos.z) * t) * cam.ZNear;

		t = (1 / cam.ZNear - 1 / A.pos.z) / (1 / C.pos.z - 1 / A.pos.z);
		newa = A;
		newa.pos = (A.pos / A.pos.z + (C.pos / C.pos.z - A.pos / A.pos.z) * t) * cam.ZNear;
		newa.u = (A.u / A.pos.z + t * (C.u / C.pos.z - A.u / A.pos.z) * t) * cam.ZNear;
		newa.v = (A.v / A.pos.z + t * (C.v / C.pos.z - A.v / A.pos.z) * t) * cam.ZNear;

		myGouraudMapping(C, newb, newa, cam, ambl, dirl);
	}
}
