#pragma once
#include "AGameObject.h"
#include "TexturedVertexBuffer.h"

class TexturedQuad: public AGameObject
{

public:
	TexturedQuad(string name, void* shaderByteCode, size_t sizeShader);
	~TexturedQuad();

public:
	void update();
	void draw(int width, int height);
	ID3D11ShaderResourceView* myTexture;
	ID3D11SamplerState* samplerState;



private:
	TexturedVertexBuffer* texturedVertexBuffer;
};

