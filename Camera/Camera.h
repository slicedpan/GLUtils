#pragma once

#include "svl/svl.h"
#include "../MiscUtils.h"

class Camera
{
public:
	Camera(void);
	~Camera(void);
	Mat4 GetTransform();
	Mat4 GetViewTransform();
	Mat4 GetProjectionMatrix();
	void SetAspectRatio(float aspectRatio);
	void SetFOV(float fieldOfView);
	void SetZNear(float zNear);
	void SetZFar(float zFar);
	float Pitch, Yaw;
	Vec3 Position;
	Vec3 GetForwardVector();
	Vec3 GetRightVector();
private:
	void UpdateProjection();
	Mat4 projection;
	float aspectRatio;
	float fov;
	float zNear;
	float zFar;
};

