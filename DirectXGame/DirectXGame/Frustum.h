#pragma once
#include "AGameObject.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"

#include "VertexShader.h"
#include "PixelShader.h"
#include "Camera.h"

class Frustum : public AGameObject
{
public:
	Frustum(string name, void* shaderByteCode, size_t sizeShader, Camera* targetCamera);
	~Frustum();

	void update(float deltaTime) override;
	void draw(int width, int height) override;
	void setAnimation(float speed, float interval, bool isSpeeding, float rotFactor);

	float m_scale_Fustrum = 1;
	float m_forward = 0.0f;
	float m_rightward = 0.0f;

	Camera* objCamera;

private:
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	ConstantBuffer* constantBuffer;

	VertexShader* vertexShader;
	PixelShader* pixelShader;

	float ticks = 0.0f;
	float deltaPos = 0.0f;
	float deltaTime = 0.0f;
	float speed = 1.0f;

	bool isIncreasing = false;
	float rotFactor = 0.0f;
	float animationInterval = 0.0f;
};
