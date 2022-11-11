#pragma once
#include <string>
#include "Vector3D.h"
#include "Vector2D.h"

#include "Matrix4x4.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include"VertexShader.h"
#include"PixelShader.h"
#include <vector>
using namespace std;

class VertexShader;
class PixelShader;

class AGameObject
{
public:
	AGameObject(string name);
	~AGameObject();

	virtual void update(float deltaTime);
	virtual void draw(int width, int height);

	void setPosition(float x, float y, float z);
	void setPosition(Vector3D pos);
	Vector3D getLocalPosition();

	void setScale(float x, float y, float z);
	void setScale(Vector3D scale);
	Vector3D getLocalScale();

	void setRotation(float x, float y, float z);
	void setRotation(Vector3D rot);
	Vector3D getLocalRotation();
	
	VertexBuffer* getVertexBuffer();
	IndexBuffer* getIndexBuffer();
	ConstantBuffer* getConstantBuffer();

	VertexShader* getVertexShader();
	PixelShader* getPixelShader();

	void reset();
	bool isSelected = false;
	string getName();

	struct Vertex
	{
		Vector3D position;
		Vector3D color;
		Vector3D color2;
	};

	struct TexturedVertex
	{
		Vector3D position;
		Vector2D texCoord;
	};


	_declspec(align(16))
		struct CBData
	{
		Matrix4x4 worldMatrix;
		Matrix4x4 viewMatrix;
		Matrix4x4 projMatrix;
		float time;
	};


protected:
	string name;
	Vector3D localPosition;
	Vector3D localScale;
	Vector3D localRotation;
	Matrix4x4 localMatrix;

	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	ConstantBuffer* constantBuffer;

	VertexShader* vertexShader;
	PixelShader* pixelShader;

	void* shaderByteCode = nullptr;
	size_t sizeShader = 0;



};

