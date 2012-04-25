#include "Camera.h"

#define M_PI 3.14159265

Camera::Camera(void) : Position(0.0f, 0.0f, 0.0f)
{
	Pitch = 0.0f;
	Yaw = 0.0f;
	fov = 0.52;
	zNear = 0.1f;
	zFar = 1000.0f;
	aspectRatio = 1.33333;
}

Camera::~Camera(void)
{
	UpdateProjection();
}

Mat4 Camera::GetTransform()
{
	Mat4 transform;
	transform.MakeHRot(Vec3(1.0f, 0.0f, 0.0f), Pitch);
	transform *= HRot4(Vec3(0.0f, 1.0f, 0.0f), Yaw);
	transform *= HTrans4(Position);
	return transform;
}

Mat4 Camera::GetViewTransform()
{
	return inv(GetTransform());
}

Vec3 Camera::GetForwardVector()
{
	Vec3 forwardVec;
	forwardVec = proj(Vec4(0.0f, 0.0f, -1.0f, 1.0f) * HRot4(Vec3(1.0f, 0.0f, 0.0f), Pitch) * HRot4(Vec3(0.0f, 1.0f, 0.0f), Yaw));
	return forwardVec;
}

Vec3 Camera::GetRightVector()
{
	Vec3 rightVec;
	rightVec = proj(Vec4(1.0f, 0.0f, 0.0, 1.0f) * HRot4(Vec3(0.0f, 1.0f, 0.0f), Yaw));
	return rightVec;
}

Mat4 Camera::GetProjectionMatrix()
{
	return projection;
}

void Camera::UpdateProjection()
{
	float f = cos(fov / 2.0) / sin(fov / 2.0);
	projection = Mat4(f / aspectRatio, 0.0, 0.0, 0.0, 0.0, f, 0.0, 0.0, 0.0, 0.0, (zFar + zNear) / (zNear - zFar), -1.0, 0.0, 0.0, (2 * zFar * zNear) / (zNear - zFar), 0.0);
}

void Camera::SetAspectRatio(float aspectRatio)
{
	this->aspectRatio = aspectRatio;
	UpdateProjection();
}

void Camera::SetFOV(float FOV)
{
	this->fov = FOV;
	UpdateProjection();
}

void Camera::SetZFar(float zFar)
{
	this->zFar = zFar;
	UpdateProjection();
}

void Camera::SetZNear(float zNear)
{
	this->zNear = zNear;
	UpdateProjection();
}