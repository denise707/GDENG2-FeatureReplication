#include "SceneCameraHandler.h"

#include <iostream>

SceneCameraHandler* SceneCameraHandler::sharedInstance = NULL;

SceneCameraHandler* SceneCameraHandler::getInstance()
{
	return sharedInstance;
}

void SceneCameraHandler::initialize()
{
	sharedInstance = new SceneCameraHandler();
}

void SceneCameraHandler::destroy()
{

}

void SceneCameraHandler::update(float deltaTime, int width, int height)
{
	sharedInstance->width = width;
	sharedInstance->height = height;
	sharedInstance->ActiveCamera->update(deltaTime, width, height);
}

Matrix4x4 SceneCameraHandler::getSceneCameraViewMatrix()
{
	return sharedInstance->ActiveCamera->getViewMatrix();
}

Camera* SceneCameraHandler::getActiveCamera()
{
	return this->ActiveCamera;
}

Camera* SceneCameraHandler::getSceneCamera()
{
	return this->sceneCam;
}

Camera* SceneCameraHandler::getPlayerCamera()
{
	return this->playerCam;
}


void SceneCameraHandler::SetPlayerCamera(Camera* playerCam)
{
	this->playerCam = playerCam;
}

void SceneCameraHandler::switchCamera(bool isUsingPlayerCam)
{
	this->ActiveCamera->setCameraStatus(false);

	if (isUsingPlayerCam)
	{
		this->ActiveCamera = playerCam;
	}
	else
	{
		this->ActiveCamera = sceneCam;
	}
	cout << ActiveCamera->getName() << "\n";
	this->ActiveCamera->setCameraStatus(true);

}

SceneCameraHandler::SceneCameraHandler()
{
	this->sceneCam = new Camera("SceneCamera");
	this->ActiveCamera = sceneCam;
	this->ActiveCamera->setCameraStatus(true);
	cout << ActiveCamera->getName() << "\n";


}

SceneCameraHandler::~SceneCameraHandler()
{
}
