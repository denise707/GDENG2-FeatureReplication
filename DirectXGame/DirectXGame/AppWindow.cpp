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

	shader_byte_code = nullptr;
	size_shader = 0;

	//Vertex Shader
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);

	//Create Object and Gizmo Instances
	for(int i = 0; i < 3; i++)
	{
		//Cube 
		Cube* cube = new Cube("Cube", shader_byte_code, size_shader);
		this->objList.push_back((cube));

		//Gizmo
		OutlineGizmo* outline = new OutlineGizmo("OutlineGizmo", shader_byte_code, size_shader);
		this->outlineList.push_back((outline));
	}

	//Set Object Transform 
	this->objList[1]->setPosition(1, 0, 0);
	this->objList[2]->setPosition(0, 0, 1);
	
	//Outline Gizmo Transform (Copy Corresponding Object's Transform)
	for (int i = 0; i < outlineList.size(); i++) {
		this->outlineList[1]->setPosition(1, 0, 0);
		this->outlineList[2]->setPosition(0, 0, 1);
	}

	//Release Compiled Shader
	GraphicsEngine::get()->releaseCompiledShader();

	//Pixel Shader
	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

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

	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_ps);

	//UPDATE CAMERA
	SceneCameraHandler::getInstance()->update(EngineTime::getDeltaTime(), width, height);

	//Outline Selected Objects
	#pragma region Outline Selected Objects

	//Updated Selected Objects
	for (int i = 0; i < selectedObjList.size(); i++) {
		objList[i]->isSelected = selectedObjList[i];
	}

	//Set Stencil State to Write
	GraphicsEngine::get()->createStencilState("Write");
	GraphicsEngine::get()->getImmediateDeviceContext()->setDepthStencilState();

	//Draw Selected Objects
	for (int i = 0; i < objList.size(); i++)
	{
		if (objList[i]->isSelected)
			objList[i]->draw(width, height, m_vs, m_ps);
	}

	//Set Stencil State to Off
	GraphicsEngine::get()->createStencilState("Off");
	GraphicsEngine::get()->getImmediateDeviceContext()->setDepthStencilState();

	//Draw All Objects
	for (int i = 0; i < objList.size(); i++)
	{
		objList[i]->draw(width, height, m_vs, m_ps);
	}

	//Set Stencil State to Mask
	GraphicsEngine::get()->createStencilState("Mask");
	GraphicsEngine::get()->getImmediateDeviceContext()->setDepthStencilState();

	//Draw Outline Gizmos
	for (int i = 0; i < outlineList.size(); i++)
	{
		if (objList[i]->isSelected)
		{
			outlineList[i]->isSelected = true;
			outlineList[i]->draw(width, height, m_vs, m_ps);
		}
	}
	#pragma endregion

	//Draw UI
	UIManager::getInstance()->drawAllUI();

	m_swap_chain->present(true);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_swap_chain->release();
	m_vs->release();
	m_ps->release();
	GraphicsEngine::get()->release();
}

void AppWindow::onFocus()
{
	InputSystem::getInstance()->addListener(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::getInstance()->removeListener(this);
}

void AppWindow::onKeyDown(int key)
{

}

void AppWindow::onKeyUp(int key)
{
	
}

void AppWindow::onMouseMove(const Point deltaPos)
{
	//int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	//int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	//InputSystem::get()->showCursor(true);
	////InputSystem::get()->setCursorPosition(Point((int)(width / 2.0f), (int)(height / 2.0f)));
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