#pragma once
#include "3DDateStruct.h"

class AmbtLight
{
public:
	int r;
	int g;
	int b;

	AmbtLight(int rr, int gg, int bb) : r(rr), g(gg), b(bb) {}
	AmbtLight() {}

};
class DirectLight
{
public:
	int r;
	int g;
	int b;
	Vector4 direction;

	DirectLight(int rr, int gg, int bb, const Vector4& dir) : r(rr), g(gg), b(bb), direction(dir) {}
	DirectLight() {}

};
