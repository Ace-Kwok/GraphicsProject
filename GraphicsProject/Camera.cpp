#include "Camera.h"

Camera::Camera(Vector4 postion, Vector4 upVector, Vector4 dirVector, double _fov, double _aspect, double zNear, double zFar) :
	pos(postion),
	up(upVector),
	dir(dirVector),
	fov(_fov),
	aspect(_aspect),
	ZNear(zNear),
	ZFar(zFar)
{
	right = upVector.CrossProduct(dirVector).normalize();
	mViewTransform = Matrix4x4::BuildLookAtLH(postion, upVector, dirVector);
	mProjectTransform = Matrix4x4::BuildPerspectiveLH(_fov, _aspect, zNear, zFar);
}

void Camera::move(double x = 0, double y = 0, double z = 0)
{
	pos = pos + right * x + up * y + dir * z;
	updateMatrix();
}

void Camera::rotate(double pitch = 0, double yaw = 0, double roll = 0)
{
	Matrix4x4 p = Matrix4x4::RotateOnAxis(right, pitch);
	Matrix4x4 y = Matrix4x4::RotateOnAxis(up, yaw);
	Matrix4x4 r = Matrix4x4::RotateOnAxis(dir, roll);
	up = r * y * p * up;
	right = r * y * p * right;
	dir = r * y * p * dir;
	updateMatrix();
}

void Camera::updateMatrix()
{
	mViewTransform = Matrix4x4::BuildLookAtLH(pos, up, dir);
	mProjectTransform = Matrix4x4::BuildPerspectiveLH(fov, aspect, ZNear, ZFar);
}
