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

	// Inherited via InputListener
	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;
	virtual void onMouseMove(const Point& mouse_pos) override;
	virtual void onLeftMouseDown(const Point& mouse_pos) override;
	virtual void onLeftMouseUp(const Point& mouse_pos) override;
	virtual void onRightMouseDown(const Point& mouse_pos) override;
	virtual void onRightMouseUp(const Point& mouse_pos) override;

private:
	void updateViewMatrix();

	int width = 0;
	int height = 0;

	float m_rot_x = 0.0f;
	float m_rot_y = 0.0f;

	float m_forward = 1.0f;
	float m_rightward = 1.0f;

	float m_delta_time;
	Point deltaPos;

	float x = 0;
	float y = 0;
	float z = 0;

	Matrix4x4 m_world_cam;
	Matrix4x4 originalPos;

	bool lmbDown = false;
};

