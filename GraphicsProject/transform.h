#pragma once

#include "3DDateStruct.h"
class Camera;

//Å·À­½Ç×ª»»µ½¾ØÕó
Matrix4x4 GetRotateXMatrix(double degreeX);

Matrix4x4 GetRotateYMatrix(double degreeY);

Matrix4x4 GetRotateYMatrix(double degreeZ);

Matrix4x4 GetXYZRotationMatrix(double degreeX, double degreeY, double degreeZ);

//View Transform
Vector4 World2Camera(Vector4 v, Camera cam);

//Project Transform
Vector4 Camera2View(Vector4 v, Camera cam);

//viewport Transform
Vector4 View2Screen(Vector4 v, double Height, double Width);