#include "Frustum.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "EngineTime.h"
#include "SceneCameraHandler.h"
#include "Utils.h"

#include "SwapChain.h"

Frustum::Frustum(string name, void* shaderByteCode, size_t sizeShader, Camera* targetCamera) :AGameObject(name)
{
	objCamera = targetCamera;

	// setting up camera
	static float FOV = Utils::radToDeg(objCamera->getFOV());
	float fovRadians = FOV / 180 * 3.14;
	static float nearZ = objCamera->getNearZ();
	static float farZ = objCamera->getFarZ();

	float nearHeight = 2 * tan(fovRadians / 2) * nearZ;
	float farHeight = 2 * tan(fovRadians / 2) * (farZ - nearZ);
	float nearWidth = nearHeight * objCamera->getAspectRatio();
	float farWidth = farHeight * objCamera->getAspectRatio();


#pragma region Frustum initialize

	Vector3D nearBL = Vector3D(-(nearWidth * 0.5), (-nearHeight * 0.5), nearZ / 2);
	Vector3D nearBR = Vector3D((nearWidth * 0.5), (-nearHeight * 0.5), nearZ / 2);
	Vector3D nearTL = Vector3D((-nearWidth * 0.5), (nearHeight * 0.5), nearZ / 2);
	Vector3D nearTR = Vector3D((nearWidth * 0.5), (nearHeight * 0.5), nearZ / 2);

	Vector3D farBL = Vector3D(-(farWidth * 0.5), (-farHeight * 0.5), (farZ - nearZ));
	Vector3D farBR = Vector3D((farWidth * 0.5), (-farHeight * 0.5), (farZ - nearZ));
	Vector3D farTL = Vector3D((-farWidth * 0.5), (farHeight * 0.5), (farZ - nearZ));
	Vector3D farTR = Vector3D((farWidth * 0.5), (farHeight * 0.5), (farZ - nearZ));

	//create buffers for drawing. Vertex data that needs to be drawn are temporarily placed here.
	Vertex fustrumList[] = {
		//X, Y, Z
		//FRONT FACE
		{nearBL, Vector3D(1,1,1), Vector3D(1,1,1) },//NBL
		{nearTL, Vector3D(1,1,1), Vector3D(1,1,1) },//NTL
		{nearTR, Vector3D(1,1,1), Vector3D(1,1,1) },//NTR
		{nearBR, Vector3D(1,1,1), Vector3D(1,1,1) },//NBR

		//BACK FACE
		{farBR, Vector3D(1,1,1), Vector3D(1,1,1) },//FBR
		{farTR, Vector3D(1,1,1), Vector3D(1,1,1) },//FTR
		{farTL, Vector3D(1,1,1), Vector3D(1,1,1) },//FTL
		{farBL, Vector3D(1,1,1), Vector3D(1,1,1) },//FBL
	};

	unsigned int index_list[] =
	{
		//FRONT SIDE
		0,1,2,  //FIRST TRIANGLE
		2,3,0,  //SECOND TRIANGLE
		//BACK SIDE
		4,5,6,
		6,7,4,
		//TOP SIDE
		1,6,5,
		5,2,1,
		//BOTTOM SIDE
		7,0,3,
		3,4,7,
		//RIGHT SIDE
		3,2,5,
		5,4,3,
		//LEFT SIDE
		7,6,1,
		1,0,7
	};

	unsigned int index_line_list[] =
	{
		1, 2,
		1, 0,
		0, 3,
		2, 3,

		7, 0,
		6, 7,
		6, 1,

		6, 5,
		5, 4,
		7, 4,

		5, 2,
		4, 3,
		2, 3,
	};


	// INDEX BUFFER
	indexBuffer = GraphicsEngine::get()->createIndexBuffer();
	indexBuffer->load(index_line_list, ARRAYSIZE(index_line_list));

	// VERTEX SHADER
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shaderByteCode, &sizeShader);
	vertexShader = GraphicsEngine::get()->createVertexShader(shaderByteCode, sizeShader);

	// VERTEX BUFFER
	vertexBuffer = GraphicsEngine::get()->createVertexBuffer();
	vertexBuffer->load(fustrumList, sizeof(Vertex), ARRAYSIZE(fustrumList), shaderByteCode, sizeShader);

	GraphicsEngine::get()->releaseCompiledShader();

	// PIXEL SHADER
	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shaderByteCode, &sizeShader);
	pixelShader = GraphicsEngine::get()->createPixelShader(shaderByteCode, sizeShader);
	GraphicsEngine::get()->releaseCompiledShader();

	// CONSTANT BUFFER
	CBData cc;
	cc.time = 0;

	constantBuffer = GraphicsEngine::get()->createConstantBuffer();
	constantBuffer->load(&cc, sizeof(CBData));
}

Frustum::~Frustum()
{
	this->vertexBuffer->release();
	this->indexBuffer->release();
	this->constantBuffer->release();
	this->pixelShader->release();
	this->vertexShader->release();
	AGameObject::~AGameObject();
}

void Frustum::update(float deltaTime)
{
	this->ticks += deltaTime;
	this->deltaTime = deltaTime;
}

void Frustum::draw(int width, int height)
{
	
	GraphicsEngine* graphEngine = GraphicsEngine::get();
	DeviceContext* deviceContext = graphEngine->getImmediateDeviceContext();

	CBData cbData = {};

	cbData.time = deltaTime;

	//Add object transformation
	Matrix4x4 temp;

	cbData.worldMatrix.setIdentity();

	Matrix4x4 world_cam;
	world_cam.setIdentity();

	temp.setIdentity();
	temp.setScale(getLocalScale());
	cbData.worldMatrix *= temp;

	Vector3D rotation = objCamera->getLocalRotation();

	temp.setIdentity();
	temp.setRotationX(objCamera->getLocalRotation().m_x);
	cbData.worldMatrix *= temp;

	temp.setIdentity();
	temp.setRotationY(objCamera->getLocalRotation().m_y);
	cbData.worldMatrix *= temp;

	temp.setIdentity();
	temp.setRotationZ(objCamera->getLocalRotation().m_z);
	cbData.worldMatrix *= temp;

	temp.setIdentity();
	Vector3D pos = objCamera->getLocalPosition();
	temp.setTranslation(Vector3D(pos.m_x, pos.m_y, pos.m_z + (objCamera->getNearZ() / 2)));
	cbData.worldMatrix *= temp;

	//Add camera transformation
	Matrix4x4 cameraMatrix = SceneCameraHandler::getInstance()->getSceneCameraViewMatrix();
	cbData.viewMatrix = cameraMatrix;

	Camera* ActiveCamera = SceneCameraHandler::getInstance()->getActiveCamera();
	cbData.projMatrix.setPerspectiveFovLH(ActiveCamera->getFOV(), ActiveCamera->getAspectRatio(), ActiveCamera->getNearZ(), ActiveCamera->getFarZ());

	this->constantBuffer->update(deviceContext, &cbData);

	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(vertexShader);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(pixelShader);

	//SET DEFAULT BUFFER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	deviceContext->setConstantBuffer(vertexShader, this->constantBuffer);
	deviceContext->setConstantBuffer(pixelShader, this->constantBuffer);

	deviceContext->setIndexBuffer(this->indexBuffer);
	deviceContext->setVertexBuffer(this->vertexBuffer);

	GraphicsEngine::get()->getImmediateDeviceContext()->setSolidRenderMode();

	deviceContext->drawIndexedLineList(this->indexBuffer->getSizeIndexList(), 0, 0);
}

void Frustum::setAnimation(float speed, float interval, bool isSpeeding, float rotFactor)
{
	this->rotFactor = rotFactor;
	this->speed = speed;
	this->animationInterval = interval;
	this->isIncreasing = isSpeeding;
}
