#pragma once
#include "require.h"
#include "3DDateStruct.h"

class Camera
{
public:
	Vector4 pos;
	Vector4 dir, up, right;
	double ZNear, ZFar, fov, aspect;

	Matrix4x4 mViewTransform;
	Matrix4x4 mProjectTransform;

	Camera() {};
	Camera(
		Vector4 postion,
		Vector4 upVector,
		Vector4 dirVector,
		double fov,
		double aspect,
		double zNear,
		double zFar);
	
	void move(double x, double y, double z);
	void rotate(double pitch, double yaw, double roll);
	void updateMatrix();
};

