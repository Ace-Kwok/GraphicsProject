#pragma once
#include "3DDateStruct.h"
#include "Camera.h"
#include "Light.h"

struct Material
{
	double Ka;
	double Kd;
	double Ks;
	int	   n; 
	Material(){}
	Material(double _Ka, double _Kd, double _Ks, int nn) : Ka(_Ka), Kd(_Kd), Ks(_Ks), n(nn) {}
};

class Vertex
{
public:
	Vector4 pos;
	Vector4 normal;
	Material material;
	double u, v;
	double r, g, b;
	Vertex() {}
	Vertex(Vector4 p) : pos(p) {}
	Vertex(Vector4 p, Vector4 nol, Material m, float uu, float vv, double rr = 255, double gg = 255, double bb = 255) : pos(p), normal(nol), material(m), u(uu), v(vv), r(rr), g(gg), b(bb) {}
	void setColor(const Camera& cam, const AmbtLight& ambl, const DirectLight& dirl);
};

class Triangle
{
public:
	Vertex ver[3];
	Triangle() {}
	Triangle(const Vertex& ver0, const Vertex& ver1, const Vertex& ver2)
	{
		ver[0] = ver0;
		ver[1] = ver1;
		ver[2] = ver2;
	}

};
