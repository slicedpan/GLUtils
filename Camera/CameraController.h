#pragma once

#include "svl/svl.h"
#include "../MiscUtils.h"

class Camera;

class CameraController
{
public:
	CameraController();
	~CameraController();
	float MaxSpeed;
	float PitchAngularVelocity;
	float YawAngularVelocity;
	bool HasMomentum;
	bool HasAngularMomentum;
	float LinearFriction;
	float AngularFriction;
	void Update(float msSinceLastUpdate);
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void MoveForward();
	void MoveBackward();
	void ChangeYaw(float amount);
	void ChangePitch(float amount);
	bool RestrictToXZPlane;
	void SetCamera(Camera* camera);
	const Vec3& Velocity;
private:
	Vec3 velocity;
	Vec3 forwardVec;
	Vec3 rightVec;
	Camera * camera;
	float pitchVel;
	float yawVel;
	float maxPitch;
};

