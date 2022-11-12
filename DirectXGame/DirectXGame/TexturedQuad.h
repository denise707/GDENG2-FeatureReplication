#pragma once
#include "AGameObject.h"

#include <vector>



class TexturedQuad : public AGameObject
{
public:
	TexturedQuad(string name, string texture);
	//Cube(vector<Vertex> v1, vector<Vertex> v2, string name, void* shaderByteCode, size_t sizeShader);
	~TexturedQuad();

	void update(float deltaTime) override;
	void draw(int width, int height) override;
	void drawGizmo(int width, int height) override;
	void setAnimSpeed(float speed);

	void setLookAt(Vector3D lookAtPos);

	bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);

	ID3D11ShaderResourceView* myTexture;
	ID3D11SamplerState* samplerState;

	//vector<Vertex> getVertexList();
private:
	float speed = 4.0f;
	float deltaTime = 0;
	Matrix4x4 m_world_matrix;
	TVertexBuffer* tVertexBuffer;
	//vector<Vertex> vertex_list_copy;
};

