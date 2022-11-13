#include "Camera.h"
#include "InputSystem.h"
#include "EngineTime.h"
#include <iostream>

Camera::Camera(string name) : AGameObject(name)
{
	this->setPosition(0.0f, 0.0f, -4.0f);
	m_world_cam.setTranslation(this->getLocalPosition());
	this->updateViewMatrix();
	InputSystem::getInstance()->addListener(this);
	originalPos = this->localMatrix;
};

Camera::~Camera()
{
}

void Camera::update(float delta_time, int width, int height)
{
	if (isActive)
	{
		this->width = width;
		this->height = height;
		m_delta_time = delta_time;

		Vector3D localPos = this->getLocalPosition();
		x = localPos.m_x;
		y = localPos.m_y;
		z = localPos.m_z;

		// keu input for camerae
		if (InputSystem::getInstance()->isKeyDown('W'))
		{
			m_forward = 1; //forward
		}
		else if (InputSystem::getInstance()->isKeyDown('S'))
		{
			m_forward = -1; //backwards
		}
		else if (InputSystem::getInstance()->isKeyDown('A'))
		{
			m_rightward = -1; //sideward (left)
		}
		else if (InputSystem::getInstance()->isKeyDown('D'))
		{
			m_rightward = 1; //sidewards (right)
		}
		else if (InputSystem::getInstance()->isKeyDown('E'))
		{
			m_upward = 1; //upwards
		}
		else if (InputSystem::getInstance()->isKeyDown('Q'))
		{
			m_upward = -1;//downwards
		}

		updateViewMatrix();
	}
	


}

Matrix4x4 Camera::getViewMatrix()
{
	return this->localMatrix;
}


void Camera::setCameraStatus(bool flag)
{
	isActive = flag;
}

void Camera::onKeyDown(int key)
{
	if (this->isActive)std::cout << " Object: " " \n";

	//if (key == 'W')
	//{
	//	m_forward = 1.0f;
	//	this->updateViewMatrix();
	//	
	//}
	//else if (key == 'S')
	//{
	//	m_forward = -1.0f;
	//	this->updateViewMatrix();
	//
	//}
	//else if (key == 'A')
	//{
	//	m_rightward = -1.0f;
	//	this->updateViewMatrix();
	//	
	//}
	//else if (key == 'D')
	//{
	//	m_rightward = 1.0f;
	//	this->updateViewMatrix();
	//}
	//else if (key == 'E')
	//{
	//	m_upward = 1.0f;
	//	this->updateViewMatrix();
	//}
	//else if (key == 'Q')
	//{
	//	m_upward = -1.0f;
	//	this->updateViewMatrix();
	//}
}

void Camera::onKeyUp(int key)
{
	m_forward = 0;
	m_rightward = 0;
	m_upward = 0;
}

void Camera::onMouseMove(const Point deltaPos)
{
	
	if (this->mouseDown && isActive)
	{
		Vector3D v = this->getLocalRotation();
		// may add speed factor
		v.m_x += (deltaPos.m_y * EngineTime::getDeltaTime() * 0.1f);
		v.m_y += (deltaPos.m_x * EngineTime::getDeltaTime() * 0.1f);

		// update rotation
		this->setRotation(v);

		// centering mouse pos
		//InputSystem::get()->setCursorPosition(Point((int)(width / 2.0f), (int)(height / 2.0f)));
	}
}

void Camera::onLeftMouseDown(const Point deltaPos)
{
}

void Camera::onLeftMouseUp(const Point deltaPos)
{
}

void Camera::onRightMouseDown(const Point deltaPos)
{
	this->mouseDown = true;
}

void Camera::onRightMouseUp(const Point deltaPos)
{
	this->mouseDown = false;
}


void Camera::updateViewMatrix()
{
	Matrix4x4 temp; temp.setIdentity();
	Matrix4x4 world_cam;
	world_cam.setIdentity();

	temp.setIdentity();
	temp.setRotationX(getLocalRotation().m_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(getLocalRotation().m_y);
	world_cam *= temp;

	temp.setIdentity();
	temp.setTranslation(getLocalPosition());
	world_cam *= temp;

	Vector3D new_pos;
	// z position
	new_pos= m_world_cam.getTranslation() + world_cam.getZDirection() * (m_forward * 0.1f);
	// x position
	new_pos = new_pos + world_cam.getXDirection() * (m_rightward * 0.1f);
	// y position
	new_pos = new_pos + world_cam.getYDirection() * (m_upward * 0.1f);

	this->setPosition(new_pos);
	world_cam.setTranslation(new_pos);

	m_world_cam = world_cam;

	world_cam.inverse();

	this->localMatrix = world_cam;
}

float Camera::getFOV()
{
	return this->FOV;
}

float Camera::getNearZ()
{
	return this->nearZ;
}

float Camera::getFarZ()
{
	return this->farZ;
}

float Camera::getAspectRatio()
{
	return aspectRatio;
}

void Camera::setFOV(float FOV)
{
	this->FOV = FOV;
}

void Camera::setNearZ(float nz)
{
	this->nearZ = nz;
}

void Camera::setFarZ(float fz)
{
	this->farZ = fz;
}

void Camera::setAspect(float aspectRatio)
{
	this->aspectRatio = aspectRatio;
}


