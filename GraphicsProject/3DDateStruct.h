#pragma once
#include "require.h"
#include <string.h> 

class Vector4
{
public:
	double x;
	double y;
	double z;
	int w;   // 0表示向量 1表示点

	Vector4();

	Vector4(double, double, double, int);
	
	Vector4& operator=(const Vector4& v);

	Vector4 operator+(const Vector4& v) const;

	Vector4 operator-(const Vector4& v) const;

	Vector4 operator*(double f) const;

	Vector4 operator/(double f) const;

	Vector4 CrossProduct(const Vector4& v) const;

	double DotProduct(const Vector4& v) const;

	Vector4& normalize();

	Vector4& simplify();
};

class Matrix4x4
{
public:
	double m[4][4];

	Matrix4x4();

	Matrix4x4(double mat[4][4]);

	Matrix4x4(double, double, double, double,
		double, double, double, double,
		double, double, double, double,
		double, double, double, double);

	double* operator[](int k);

	Matrix4x4& operator=(const Matrix4x4& m);

	Matrix4x4 operator*(const Matrix4x4& m);

	Matrix4x4& operator*=(const Matrix4x4& m);

	Vector4 operator*(const Vector4& v);

	Matrix4x4 Transpose();

	static Matrix4x4 CreateRotateX(double angle);
	static Matrix4x4 CreateRotateY(double angle);
	static Matrix4x4 CreateRotateZ(double angle);

	static Matrix4x4 RotateOnAxis(Vector4& axis, double angle);

	static Matrix4x4 BuildWorldTransform(
		const Vector4& postion,
		double xAngle,
		double yAngle,
		double zAngle);

	static Matrix4x4 BuildLookAtLH(
		const Vector4& position,
		const Vector4& upVector,
		const Vector4& dirVector);

	static Matrix4x4 BuildPerspectiveLH(
		double fov,
		double aspect,
		double zNear,
		double zFar);

	static Matrix4x4 BuildViewPort(
		double width,
		double height);

};