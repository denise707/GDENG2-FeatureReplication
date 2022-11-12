#pragma once
#include "AGameObject.h"

#include <vector>

class Pyramid : public AGameObject
{
public:
	Pyramid(string name);
	//Cube(vector<Vertex> v1, vector<Vertex> v2, string name, void* shaderByteCode, size_t sizeShader);
	~Pyramid();

	void update(float deltaTime) override;
	void draw(int width, int height) override;
	void drawGizmo(int width, int height) override;
	void drawBox(int width, int height) override;
	void setAnimSpeed(float speed);
	//vector<Vertex> getVertexList();
private:
	float speed = 4.0f;
	float deltaTime = 0;
	Matrix4x4 m_world_matrix;
	//vector<Vertex> vertex_list_copy;
};

