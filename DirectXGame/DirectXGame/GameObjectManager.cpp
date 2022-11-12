#include "GameObjectManager.h"
#include "GraphicsEngine.h"
#include "UIManager.h"
#include "OutlineGizmo.h"
#include "MergedActor.h"
#include <iostream>

GameObjectManager::GameObjectManager()
{
}

GameObjectManager::~GameObjectManager()
{
}

void GameObjectManager::initialize()
{
	width = UIManager::WINDOW_WIDTH;
	height = UIManager::WINDOW_HEIGHT;
}

void GameObjectManager::createCube()
{
	Cube* cube = new Cube("Cube");
	objList.push_back((cube));
	selectedObjList.push_back(cube->isSelected);
}

void GameObjectManager::changePosition(int i, float x, float y, float z)
{
	objList[i]->setPosition(x, y, z);
}

void GameObjectManager::mergeSelected()
{
	vector<AGameObject*> toMerge;
	for (int i = 0; i < objList.size(); i++) {
		if (objList[i]->isSelected) {
			toMerge.push_back(objList[i]);
		}
	}
	MergedActor* m = new MergedActor("Merged", toMerge);
	mergedObjList.push_back(m);
}

void GameObjectManager::instantiateMergedActor(int i)
{
	selectedObjList.push_back(false);
	objList.push_back(mergedObjList[i]);
}

void GameObjectManager::updateObjects()
{
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

	//Draw Outline Gizmos
	for (int i = 0; i < objList.size(); i++)
	{
		if (objList[i]->isSelected)
		{
			objList[i]->drawGizmo(width, height);
		}
	}
#pragma endregion
}

GameObjectManager* GameObjectManager::get()
{
	static GameObjectManager gOManager;
	return &gOManager;
}
