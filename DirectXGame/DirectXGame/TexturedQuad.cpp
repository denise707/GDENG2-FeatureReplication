#include "TexturedQuad.h"
#include <iostream>
#include "DeviceContext.h"
#include "GraphicsEngine.h"
#include "EngineTime.h"
#include "SceneCameraHandler.h"
#include <d3d11.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <Windows.h>


//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"


TexturedQuad::TexturedQuad(string name, void* shaderByteCode, size_t sizeShader) :AGameObject(name)
{

	TexturedVertex vertex_list [] =
	{
		//RECTANGLE          RAINBOW
		//X - Y - Z          R-G-B
		{Vector3D(-0.5f,0.1f, 0.0f) ,  Vector2D(0,1)}, // LL
		{ Vector3D(-0.5f, 0.5f, 0.0f), Vector2D(0,0)}, // UL
		{ Vector3D(-0.1f,0.1f, 0.0f),  Vector2D(1,1)},// LR
		{ Vector3D(-0.1f, 0.5f, 0.0f),  Vector2D(1,0)} // UR
	};

	//Vertex Shader
	GraphicsEngine::get()->compileVertexShader(L"TexturedVertexShader.hlsl", "main", &shaderByteCode, &sizeShader);
	vertexShader = GraphicsEngine::get()->createVertexShader(shaderByteCode, sizeShader);

	//Vertex Buffer
	this->texturedVertexBuffer = GraphicsEngine::get()->createTexturedVertexBuffer();
	this->texturedVertexBuffer->load(vertex_list, sizeof(TexturedVertex), ARRAYSIZE(vertex_list), shaderByteCode, sizeShader);

	GraphicsEngine::get()->releaseCompiledShader();

	//Pixel Shader
	GraphicsEngine::get()->compilePixelShader(L"TexturedPixelShader.hlsl", "main", &shaderByteCode, &sizeShader);
	pixelShader = GraphicsEngine::get()->createPixelShader(shaderByteCode, sizeShader);
	GraphicsEngine::get()->releaseCompiledShader();

	//Create constant buffer
	CBData cbData = {};
	cbData.time = 0;
	this->constantBuffer = GraphicsEngine::get()->createConstantBuffer();
	this->constantBuffer->load(&cbData, sizeof(CBData));

	/*int width = 0;
	int height = 0;
	bool ret = LoadTextureFromFile("C://Users//Setiel Olivarez/Desktop/School/GDENG2/Project/Game Engine/dlsu.png", &myTexture, &width, &height);
	IM_ASSERT(ret);*/

}

TexturedQuad::~TexturedQuad()
{
	this->texturedVertexBuffer->release();
	//this->vertexShader->release();
	//this->pixelShader->release();
	this->constantBuffer->release();

}

void TexturedQuad::update()
{
}

void TexturedQuad::draw(int width, int height)
{

	GraphicsEngine* graphEngine = GraphicsEngine::get();
	DeviceContext* deviceContext = graphEngine->getImmediateDeviceContext();

	CBData cbData = {};

	//cbData.time = deltaTime;

	//Add object transformation
	Matrix4x4 temp;

	cbData.worldMatrix.setIdentity();

	Matrix4x4 world_cam;
	world_cam.setIdentity();

	temp.setIdentity();
	temp.setScale(getLocalScale());
	cbData.worldMatrix *= temp;

	temp.setIdentity();
	temp.setRotationX(getLocalRotation().m_x);
	cbData.worldMatrix *= temp;

	temp.setIdentity();
	temp.setRotationY(getLocalRotation().m_y);
	cbData.worldMatrix *= temp;

	temp.setIdentity();
	temp.setRotationZ(getLocalRotation().m_z);
	cbData.worldMatrix *= temp;

	temp.setIdentity();
	temp.setTranslation(getLocalPosition());
	cbData.worldMatrix *= temp;

	//Add camera transformation
	Matrix4x4 cameraMatrix = SceneCameraHandler::getInstance()->getSceneCameraViewMatrix();
	cbData.viewMatrix = cameraMatrix;

	//Perspective View
	cbData.projMatrix.setPerspectiveFovLH(1.57f, ((float)width / (float)height), 0.1f, 100.0f);

	this->constantBuffer->update(deviceContext, &cbData);

	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShaderSamplers(0,1, samplerState);
	GraphicsEngine::get()->getImmediateDeviceContext()->setShaderResources(0, 1, this->myTexture);


	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(vertexShader);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(pixelShader);

	//SET THE VERTICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setTexturedVertexBuffer(texturedVertexBuffer);

	// FINALLY DRAW THE TRIANGLE
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(texturedVertexBuffer->getSizeVertexList(), 0);
}

//// Simple helper function to load an image into a DX11 texture with common settings
//bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
//{
//	ID3D11Device* g_pd3dDevice = GraphicsEngine::get()->getDevice();
//	// Load from disk into a raw RGBA buffer
//	int image_width = 0;
//	int image_height = 0;
//	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
//	if (image_data == NULL)
//		return false;
//
//	// Create texture
//	D3D11_TEXTURE2D_DESC desc;
//	ZeroMemory(&desc, sizeof(desc));
//	desc.Width = image_width;
//	desc.Height = image_height;
//	desc.MipLevels = 1;
//	desc.ArraySize = 1;
//	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//	desc.SampleDesc.Count = 1;
//	desc.Usage = D3D11_USAGE_DEFAULT;
//	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
//	desc.CPUAccessFlags = 0;
//
//	ID3D11Texture2D* pTexture = NULL;
//	D3D11_SUBRESOURCE_DATA subResource;
//	subResource.pSysMem = image_data;
//	subResource.SysMemPitch = desc.Width * 4;
//	subResource.SysMemSlicePitch = 0;
//	g_pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);
//
//	// Create texture view
//	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
//	ZeroMemory(&srvDesc, sizeof(srvDesc));
//	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
//	srvDesc.Texture2D.MipLevels = desc.MipLevels;
//	srvDesc.Texture2D.MostDetailedMip = 0;
//	g_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
//	pTexture->Release();
//
//	*out_width = image_width;
//	*out_height = image_height;
//	stbi_image_free(image_data);
//
//	return true;
//}