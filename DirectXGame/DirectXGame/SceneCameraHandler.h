#pragma once
#include "Camera.h"
#include "Matrix4x4.h"

class SceneCameraHandler
{
public:
	static SceneCameraHandler* getInstance();
	static void initialize();
	static void destroy();

	void update(float deltaTime, int width, int height);

	Matrix4x4 getSceneCameraViewMatrix();
	Camera* getActiveCamera();
	Camera* getSceneCamera();
	Camera* getPlayerCamera();

	void SetPlayerCamera(Camera* playerCam);

	void switchCamera(bool isUsingPlayerCam);

	Matrix4x4 worldmat;
	Matrix4x4 viewmat;
	Matrix4x4 projmat;

private:
	SceneCameraHandler();
	~SceneCameraHandler();
	SceneCameraHandler(SceneCameraHandler const&) {};
	SceneCameraHandler& operator=(SceneCameraHandler const&) {};
	static SceneCameraHandler* sharedInstance;

	Camera* ActiveCamera;
	Camera* sceneCam;
	Camera* playerCam;


	int width = 0;
	int height = 0;
};

