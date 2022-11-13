#pragma once
#include "AGameObject.h"
#include "InputListener.h"
#include "Window.h"

class Camera: public AGameObject, public InputListener
{
public:
	Camera(string name);
	~Camera();

	void update(float deltaTime, int width, int height);
	Matrix4x4 getViewMatrix();
	void setCameraStatus(bool flag);

	//Set's Ver
	// Inherited via InputListener
	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;
	virtual void onMouseMove(const Point deltaPos) override;
	virtual void onLeftMouseDown(const Point deltaPos) override;
	virtual void onLeftMouseUp(const Point deltaPos) override;
	virtual void onRightMouseDown(const Point deltaPos) override;
	virtual void onRightMouseUp(const Point deltaPos) override;


	float getFOV();
	float getNearZ();
	float getFarZ();
	float getAspectRatio();


	void setFOV(float fov);
	void setNearZ(float nz);
	void setFarZ(float fz);
	void setAspect(float aspectRatio);



private:
	void updateViewMatrix();

	int width = 0;
	int height = 0;

	float m_rot_x = 0.0f;
	float m_rot_y = 0.0f;

	float m_forward = 1.0f;
	float m_rightward = 1.0f;
	float m_upward = 1.0f;


	float m_delta_time;
	Point deltaPos;

	float x = 0;
	float y = 0;
	float z = 0;

	Matrix4x4 m_world_cam;
	Matrix4x4 originalPos;
	bool isActive = false;


	float mouseDown = false;

	float FOV = 0;
	float nearZ = 0;
	float farZ = 0;
	float aspectRatio = 0;



};

