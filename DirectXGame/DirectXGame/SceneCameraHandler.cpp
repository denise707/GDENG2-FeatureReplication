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
	sharedInstance->sceneCamera->update(deltaTime, width, height);
}

Matrix4x4 SceneCameraHandler::getSceneCameraViewMatrix()
{
	return sharedInstance->sceneCamera->getViewMatrix();
}

Camera* SceneCameraHandler::getSceneCamera()
{
	return this->sceneCamera;
}

SceneCameraHandler::SceneCameraHandler()
{
	this->sceneCamera = new Camera("Camera");
}

SceneCameraHandler::~SceneCameraHandler()
{
}
