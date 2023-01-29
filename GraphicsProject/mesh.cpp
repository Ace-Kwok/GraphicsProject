#include "mesh.h"

void Vertex::setColor(const Camera& cam, const AmbtLight& ambl, const DirectLight& dirl)
{
	AmbtLight Ia = ambl;
	DirectLight Ip = dirl;

	Vector4 L(Ip.direction * -1);
	L.normalize();
	Vector4 N = normal;
	N.normalize();

	Vector4 V = cam.pos - pos;
	V.normalize();
	Vector4 H = (L + V) / 2;
	H.normalize();

	int Ir;
	int Ig;
	int Ib;

	Material m = material;

	double factor = m.Kd * (L.DotProduct(N)) + m.Ks * pow((H.DotProduct(N)), m.n);
	factor = factor < 0 ? 0 : factor;

	Ir = m.Ka * Ia.r + Ip.r * factor;
	if (Ir < 0) Ir = 0;
	else if (Ir > 255) Ir = 255;
	Ig = m.Ka * Ia.g + Ip.g * factor;
	if (Ig < 0) Ig = 0;
	else if (Ig > 255) Ig = 255;
	Ib = m.Ka * Ia.b + Ip.b * factor;
	if (Ib < 0) Ib = 0;
	else if (Ib > 255) Ib = 255;

	r = Ir;
	g = Ig;
	b = Ib;
}