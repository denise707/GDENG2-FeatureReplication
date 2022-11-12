#include "MergedActor.h"
#include <iostream>
#include "DeviceContext.h"
#include "EngineTime.h"
#include "GraphicsEngine.h"
#include "SceneCameraHandler.h"
#include <iostream>()
//Utils* utils = new Utils();
MergedActor::MergedActor(string name, void* shaderByteCode, size_t sizeShader, AGameObject* obj1, AGameObject* obj2) :AGameObject(name)
{

	//Create constant buffer
	CBData cbData = {};
	cbData.time = 0;
	this->constantBuffer = GraphicsEngine::get()->createConstantBuffer();
	this->constantBuffer->load(&cbData, sizeof(CBData));

	setAnimSpeed(4);


	if (obj1 == nullptr)
	{
		std::cout << "obj1 null\n";
	}
	AGameObject* obj1Copy = new AGameObject("copy1"); memcpy(obj1Copy, obj1, sizeof(AGameObject));
	AGameObject* obj2Copy = new AGameObject("copy2"); memcpy(obj2Copy, obj2, sizeof(AGameObject));


	//VertexBuffer* vertexBuffer1Copy = GraphicsEngine::get()->createVertexBuffer();
	//IndexBuffer* indexBuffer1Copy = GraphicsEngine::get()->createIndexBuffer();
	//ConstantBuffer* constantBuffer1Copy = GraphicsEngine::get()->createConstantBuffer();

	//VertexBuffer* vertexBuffer2Copy = GraphicsEngine::get()->createVertexBuffer();
	//IndexBuffer* indexBuffer2Copy = GraphicsEngine::get()->createIndexBuffer();
	//ConstantBuffer* constantBuffer2Copy = GraphicsEngine::get()->createConstantBuffer();

	//memcpy(vertexBuffer1Copy, obj1->getConstantBuffer(), sizeof(vertexBuffer));
	//memcpy(indexBuffer1Copy, obj1->getConstantBuffer(), sizeof(indexBuffer));
	//memcpy(constantBuffer1Copy, obj1->getConstantBuffer(), sizeof(ConstantBuffer));


	//memcpy(vertexBuffer2Copy, obj2->getConstantBuffer(), sizeof(vertexBuffer));
	//memcpy(indexBuffer2Copy, obj2->getConstantBuffer(), sizeof(indexBuffer));
	//memcpy(constantBuffer2Copy, obj2->getConstantBuffer(), sizeof(ConstantBuffer));


	/*vertexBuffer_table[obj1] = vertexBuffer1Copy;
	indexBuffer_table[obj1] = indexBuffer1Copy;
	constantBuffer_table[obj1] = constantBuffer1Copy;

	vertexBuffer_table[obj2] = vertexBuffer1Copy;
	indexBuffer_table[obj2] = indexBuffer1Copy;
	constantBuffer_table[obj2] = constantBuffer1Copy;*/


	objList.push_back(obj1Copy);
	objList.push_back(obj2Copy);


}

MergedActor::MergedActor(string name, vector<AGameObject*> toCombine) : AGameObject(name)
{
	//Create constant buffer
	CBData cbData = {};
	cbData.time = 0;
	this->constantBuffer = GraphicsEngine::get()->createConstantBuffer();
	this->constantBuffer->load(&cbData, sizeof(CBData));

	setAnimSpeed(4);

	for (int i = 0; i < toCombine.size(); i++)
	{
		AGameObject* objCopy = new AGameObject("copy"); memcpy(objCopy, toCombine[i], sizeof(AGameObject));
		objList.push_back(objCopy);

	}

}

MergedActor::~MergedActor()
{
	this->constantBuffer->release();
}

void MergedActor::update(float delta_time)
{

}

void MergedActor::draw(int width, int height)
{
	GraphicsEngine* graphEngine = GraphicsEngine::get();
	DeviceContext* deviceContext = graphEngine->getImmediateDeviceContext();

	for (int i = 0; i < objList.size(); i++)
	{
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

		temp.setIdentity();
		temp.setRotationX(getLocalRotation().m_x + objList[i]->getLocalRotation().m_x);
		cbData.worldMatrix *= temp;

		temp.setIdentity();
		temp.setRotationY(getLocalRotation().m_y + objList[i]->getLocalRotation().m_y);
		cbData.worldMatrix *= temp;

		temp.setIdentity();
		temp.setRotationZ(getLocalRotation().m_z + objList[i]->getLocalRotation().m_z);
		cbData.worldMatrix *= temp;

		temp.setIdentity();
		temp.setTranslation(getLocalPosition() + objList[i]->getLocalPosition());
		cbData.worldMatrix *= temp;

		localMatrix = cbData.worldMatrix;
		//Add camera transformation
		Matrix4x4 cameraMatrix = SceneCameraHandler::getInstance()->getSceneCameraViewMatrix();
		cbData.viewMatrix = cameraMatrix;

		//Perspective View
		cbData.projMatrix.setPerspectiveFovLH(1.57f, ((float)width / (float)height), 0.1f, 100.0f);

		this->constantBuffer->update(deviceContext, &cbData);
		deviceContext->setConstantBuffer(objList[i]->getVertexShader(), this->constantBuffer);
		deviceContext->setConstantBuffer(objList[i]->getPixelShader(), this->constantBuffer);

		deviceContext->setIndexBuffer(objList[i]->getIndexBuffer());
		deviceContext->setVertexBuffer(objList[i]->getVertexBuffer());

		deviceContext->drawIndexedTriangleList(objList[i]->getIndexBuffer()->getSizeIndexList(), 0, 0);
	}

}

void MergedActor::drawGizmo(int width, int height)
{
	GraphicsEngine* graphEngine = GraphicsEngine::get();
	DeviceContext* deviceContext = graphEngine->getImmediateDeviceContext();

	for (int i = 0; i < objList.size(); i++)
	{
		CBData cbData = {};

		cbData.time = deltaTime;

		//Add object transformation
		Matrix4x4 temp;

		cbData.worldMatrix.setIdentity();

		Matrix4x4 world_cam;
		world_cam.setIdentity();

		temp.setIdentity();
		temp.setScale(getLocalScale() * 1.05f);
		cbData.worldMatrix *= temp;

		temp.setIdentity();
		temp.setRotationX(getLocalRotation().m_x + objList[i]->getLocalRotation().m_x);
		cbData.worldMatrix *= temp;

		temp.setIdentity();
		temp.setRotationY(getLocalRotation().m_y + objList[i]->getLocalRotation().m_y);
		cbData.worldMatrix *= temp;

		temp.setIdentity();
		temp.setRotationZ(getLocalRotation().m_z + objList[i]->getLocalRotation().m_z);
		cbData.worldMatrix *= temp;

		temp.setIdentity();
		temp.setTranslation(getLocalPosition() + objList[i]->getLocalPosition());
		cbData.worldMatrix *= temp;

		localMatrix = cbData.worldMatrix;
		//Add camera transformation
		Matrix4x4 cameraMatrix = SceneCameraHandler::getInstance()->getSceneCameraViewMatrix();
		cbData.viewMatrix = cameraMatrix;

		//Perspective View
		cbData.projMatrix.setPerspectiveFovLH(1.57f, ((float)width / (float)height), 0.1f, 100.0f);

		this->constantBuffer->update(deviceContext, &cbData);
		deviceContext->setConstantBuffer(objList[i]->getVertexShader(), this->constantBuffer);
		deviceContext->setConstantBuffer(objList[i]->getPixelShader(), this->constantBuffer);

		deviceContext->setIndexBuffer(objList[i]->getIndexBuffer());
		deviceContext->setVertexBuffer(objList[i]->getGizmoVertexBuffer());

		deviceContext->drawIndexedTriangleList(objList[i]->getIndexBuffer()->getSizeIndexList(), 0, 0);
		std::cout << "11\n";
	}

}

void MergedActor::setAnimSpeed(float speed)
{
	this->speed = speed;
}


