#include "3DDateStruct.h"
Vector4::Vector4()
{
	x = y = z = 0;
	w = 1;
}

Vector4::Vector4(double xx, double yy, double zz, int ww = 1)
	: x(xx)
	, y(yy)
	, z(zz)
	, w(ww)
{}

Vector4& Vector4::operator=(const Vector4& v) 
{
	x = v.x; y = v.y; z = v.z; w = v.w;
	return *this;
}

Vector4 Vector4::operator+(const Vector4& v) const
{
	return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
}

Vector4 Vector4::operator-(const Vector4& v) const 
{
	if (!v.w) return *this;
	return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
}

Vector4 Vector4::operator*(double f) const
{
	return Vector4(x * f, y * f, z * f, w);
}

Vector4 Vector4::operator/(double f) const 
{
	double inv = 1.f / f;
	return Vector4(x * inv, y * inv, z * inv, w * inv);
}

Vector4 Vector4::CrossProduct(const Vector4& v) const
{
	Vector4 ret;
	ret.x = y * v.z - z * v.y;
	ret.y = z * v.x - x * v.z;
	ret.z = x * v.y - y * v.x;
	ret.w = 0;
	return ret;
}

double Vector4::DotProduct(const Vector4& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

Vector4& Vector4::normalize()
{
	//if (!this->w) return *this;
	double f = sqrtf(x * x + y * y + z * z);
	x /= f;
	y /= f;
	z /= f;
	return *this;
}

Vector4& Vector4::simplify()
{
	if (!w) return *this;
	x /= w;
	y /= w;
	z /= w;
	w = 1;
	return *this;
}




Matrix4x4::Matrix4x4()
{
	m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.f;
	m[0][1] = m[0][2] = m[0][3] =
	m[1][0] = m[1][2] = m[1][3] =
	m[2][0] = m[2][1] = m[2][3] =
	m[3][0] = m[3][1] = m[3][2] = 0.f;
}

Matrix4x4::Matrix4x4(double mat[4][4]) 
{
	memcpy(m, mat, 16 * sizeof(double));
}

Matrix4x4::Matrix4x4(double m00, double m01, double m02, double m03,
	double m10, double m11, double m12, double m13,
	double m20, double m21, double m22, double m23,
	double m30, double m31, double m32, double m33) 
{
	m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
	m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
	m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
	m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
}

double* Matrix4x4::operator[](int k) 
{
	return m[k];
}

Matrix4x4& Matrix4x4::operator=(const Matrix4x4& mm)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			m[i][j] = mm.m[i][j];
	return *this;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& mm)
{
	Matrix4x4 r;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			r.m[i][j] = m[i][0] * mm.m[0][j] + m[i][1] * mm.m[1][j] + m[i][2] * mm.m[2][j] + m[i][3] * mm.m[3][j];
	return r;
}

Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& mm)
{
	Matrix4x4 r;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			r.m[i][j] = m[i][0] * mm.m[0][j] + m[i][1] * mm.m[1][j] + m[i][2] * mm.m[2][j] + m[i][3] * mm.m[3][j];
	*this = r;
	return *this;
}

Vector4 Matrix4x4::operator*(const Vector4& v)
{
	Vector4 r;
	r.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w;
	r.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w;
	r.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w;
	r.w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w;
	return r;
}

Matrix4x4 Matrix4x4::Transpose()
{
	Matrix4x4 x;

	return Matrix4x4(m[0][0], m[1][0], m[2][0], m[3][0],
		m[0][1], m[1][1], m[2][1], m[3][1],
		m[0][2], m[1][2], m[2][2], m[3][2],
		m[0][3], m[1][3], m[2][3], m[3][3]);
}

Matrix4x4 Matrix4x4::CreateRotateX(double angle)
{
	double cosTheta = cos(angle * PI / 180.0);
	double sinTheta = sin(angle * PI / 180.0);
	return Matrix4x4(1, 0, 0, 0,
		0, cosTheta, -sinTheta, 0,
		0, sinTheta, cosTheta, 0,
		0, 0, 0, 1);
}

Matrix4x4 Matrix4x4::CreateRotateY(double angle)
{
	double cosTheta = cos(angle * PI / 180.0);
	double sinTheta = sin(angle * PI / 180.0);
	return Matrix4x4(cosTheta, 0, sinTheta, 0,
		0, 1, 0, 0,
		-sinTheta, 0, cosTheta, 0,
		0, 0, 0, 1);
}

Matrix4x4 Matrix4x4::CreateRotateZ(double angle)
{
	double cosTheta = cos(angle * PI / 180.0);
	double sinTheta = sin(angle * PI / 180.0);
	return Matrix4x4(cosTheta, -sinTheta, 0, 0,
		sinTheta, cosTheta, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
}

Matrix4x4 Matrix4x4::RotateOnAxis(Vector4& axis, double angle)
{
	axis.normalize();
	double u = axis.x;
	double v = axis.y;
	double w = axis.z;
	return Matrix4x4(
		cos(angle * PI / 180.0) + (u * u) * (1 - cos(angle * PI / 180.0)), u * v * (1 - cos(angle * PI / 180.0)) - w * sin(angle * PI / 180.0), u * w * (1 - cos(angle * PI / 180.0)) + v * sin(angle * PI / 180.0), 0,
		u * v * (1 - cos(angle * PI / 180.0)) + w * sin(angle * PI / 180.0), cos(angle * PI / 180.0) + v * v * (1 - cos(angle * PI / 180.0)), w * v * (1 - cos(angle * PI / 180.0)) - u * sin(angle * PI / 180.0), 0,
		u * w * (1 - cos(angle * PI / 180.0)) - v * sin(angle * PI / 180.0), v * w * (1 - cos(angle * PI / 180.0)) + u * sin(angle * PI / 180.0), cos(angle * PI / 180.0) + w * w * (1 - cos(angle * PI / 180.0)), 0,
		0, 0, 0, 1
	);
}

Matrix4x4 Matrix4x4::BuildWorldTransform(const Vector4& postion, double xAngle, double yAngle, double zAngle)
{
	Matrix4x4 tanslationInv = Matrix4x4(
		1, 0, 0, postion.x,
		0, 1, 0, postion.y,
		0, 0, 1, postion.z,
		0, 0, 0, 1);

	Matrix4x4 yInv = CreateRotateY(yAngle);
	Matrix4x4 xInv = CreateRotateX(xAngle);
	Matrix4x4 zInv = CreateRotateZ(zAngle);

	return tanslationInv * yInv * xInv * zInv;
}

Matrix4x4 Matrix4x4::BuildLookAtLH(const Vector4& position, const Vector4& upVector, const Vector4& dirVector)
{
	Vector4 z = dirVector;
	z.normalize();
	Vector4 x = upVector.CrossProduct(z);
	x.normalize();
	Vector4 y = z.CrossProduct(x);
	y.normalize();
	return Matrix4x4(
		x.x, x.y, x.z, -x.DotProduct(position),
		y.x, y.y, y.z, -y.DotProduct(position),
		z.x, z.y, z.z, -z.DotProduct(position),
		0, 0, 0, 1
	);
}

Matrix4x4 Matrix4x4::BuildPerspectiveLH(double fov, double aspect, double zNear, double zFar)
{
	return Matrix4x4(
		1 / (tan(fov * 0.5 * PI / 180.0) * aspect), 0, 0, 0,
		0, 1 / tan(fov * 0.5 * PI / 180.0), 0, 0,
		0, 0, zFar / (zFar - zNear), (zFar * zNear) / (zNear - zFar),
		0, 0, 1, 0
	);
}

Matrix4x4 Matrix4x4::BuildViewPort(double width, double height)
{
	return Matrix4x4(
		width / 2, 0, 0, width / 2,
		0, -height / 2, 0, height / 2,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
}
