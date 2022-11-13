#include "AppWindow.h"
#include <Windows.h>
#include "EngineTime.h"
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "SceneCameraHandler.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include "OutlineGizmo.h"
#include "UIManager.h"
#include "GameObjectManager.h"

vector<bool> AppWindow::selectedObjList = {false, false, false};

struct vertex
{
	Vector3D position;
	Vector3D color;
	Vector3D color1;
};

__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	unsigned int m_time;
};

AppWindow::AppWindow()
{
}

void AppWindow::update()
{

}

AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	Window::onCreate();
	
	EngineTime::initialize();
	
	InputSystem::initialize();
	InputSystem::getInstance()->addListener(this);
	//InputSystem::getInstance()->showCursor(false);

	GraphicsEngine::get()->init();
	m_swap_chain = GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);
	
	//Initialize Scene Camera
	SceneCameraHandler::initialize();
	// CREATE CAMERAS
	cameraObj = new Camera("ObjCam");
	ActiveCamera = SceneCameraHandler::getInstance()->getActiveCamera();

	//UPDATE CAMERA PARAMETERS
	float aspectRatio = (float)(rc.right - rc.left) / (float)(rc.bottom - rc.top);


	cameraObj->setAspect(aspectRatio);
	cameraObj->setFOV(aspectRatio);
	cameraObj->setNearZ(1);
	cameraObj->setFarZ(10);

	ActiveCamera->setAspect(aspectRatio);
	ActiveCamera->setFOV(aspectRatio);
	ActiveCamera->setNearZ(1);
	ActiveCamera->setFarZ(100);

	SceneCameraHandler::getInstance()->SetPlayerCamera(cameraObj);


	//Create Object and Gizmo Instances
	GameObjectManager::get()->initialize();
	frustum = new Frustum("frustum", shader_byte_code, size_shader, cameraObj);

	// Initialize UIManager
	UIManager::getInstance()->initialize(Window::getHWND());

}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	InputSystem::getInstance()->update();

	//CLEAR THE RENDER TARGET 
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0, 0.3f, 0.4f, 1);

	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	RECT rc = this->getClientWindowRect();
	width = rc.right - rc.left;
	height = rc.bottom - rc.top;

	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);


	//UPDATE CAMERA
	SceneCameraHandler::getInstance()->update(EngineTime::getDeltaTime(), width, height);

	//Outline Selected Objects
	GameObjectManager::get()->updateObjects();
	frustum->draw(width, height);

	//Draw UI
	UIManager::getInstance()->drawAllUI();

	m_swap_chain->present(true);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_swap_chain->release();

	GraphicsEngine::get()->release();
}

void AppWindow::onFocus()
{
	//InputSystem::getInstance()->addListener(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::getInstance()->removeListener(this);
}

void AppWindow::onKeyDown(int key)
{
	if (key == 'F')
	{
		isUsingCameraObj = !isUsingCameraObj;
		// SWITCH SCENE CAMERA (F KEY)
		SceneCameraHandler::getInstance()->switchCamera(isUsingCameraObj);

	}
}

void AppWindow::onKeyUp(int key)
{

}

void AppWindow::onMouseMove(const Point deltaPos)
{

}

void AppWindow::onLeftMouseDown(const Point deltaPos)
{
	
}

void AppWindow::onLeftMouseUp(const Point deltaPos)
{
	
}

void AppWindow::onRightMouseDown(const Point deltaPos)
{
	
}

void AppWindow::onRightMouseUp(const Point deltaPos)
{

}
