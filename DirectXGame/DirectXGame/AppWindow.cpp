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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

	/*shader_byte_code = nullptr;
	size_shader = 0;*/

	////Vertex Shader
	//GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	//m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);

	//Create Object and Gizmo Instances
	for(int i = 0; i < 3; i++)
	{
		//Cube 
		Cube* cube = new Cube("Cube");
		this->objList.push_back((cube));

		//Gizmo
		OutlineGizmo* outline = new OutlineGizmo("OutlineGizmo");
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

	////Release Compiled Shader
	//GraphicsEngine::get()->releaseCompiledShader();

	////Pixel Shader
	//GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	//m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	//GraphicsEngine::get()->releaseCompiledShader();

	quad = new TexturedQuad("quad", shader_byte_code, size_shader);

	// Initialize UIManager
	UIManager::getInstance()->initialize(Window::getHWND());
	ID3D11ShaderResourceView* myTexture;
	ID3D11SamplerState* samplerState;




	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	GraphicsEngine::get()->getDevice()->CreateSamplerState(&desc, &samplerState);

	int width = 0;
	int height = 0;
	bool ret = LoadTextureFromFile("Logo.jpg", &myTexture, &width, &height);
	IM_ASSERT(ret);

	quad->myTexture = myTexture;
	quad->samplerState = samplerState;
	quad->setPosition(0, 0, 0);
	quad->setScale(5, 5, 5);

	//objList.push_back(quad);
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
			objList[i]->draw(width, height);
	}

	//Set Stencil State to Off
	GraphicsEngine::get()->createStencilState("Off");
	GraphicsEngine::get()->getImmediateDeviceContext()->setDepthStencilState();

	//Draw All Objects
	for (int i = 0; i < objList.size(); i++)
	{
		objList[i]->draw(width, height);
	}

	//Set Stencil State to Mask
	GraphicsEngine::get()->createStencilState("Mask");
	GraphicsEngine::get()->getImmediateDeviceContext()->setDepthStencilState();


	quad->draw(width, height);
	//Draw Outline Gizmos
	for (int i = 0; i < outlineList.size(); i++)
	{
		if (objList[i]->isSelected)
		{
			outlineList[i]->isSelected = true;
			objList[i]->drawGizmo(width, height);
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

// Simple helper function to load an image into a DX11 texture with common settings
bool AppWindow:: LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{
	ID3D11Device* g_pd3dDevice = GraphicsEngine::get()->getDevice();
	// Load from disk into a raw RGBA buffer
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create texture
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = image_width;
	desc.Height = image_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = image_data;
	subResource.SysMemPitch = desc.Width * 4;
	subResource.SysMemSlicePitch = 0;
	g_pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);

	// Create texture view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	g_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
	pTexture->Release();

	*out_width = image_width;
	*out_height = image_height;
	stbi_image_free(image_data);

	return true;
}