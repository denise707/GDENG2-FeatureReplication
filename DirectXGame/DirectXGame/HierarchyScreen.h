#pragma once
#include "AUIScreen.h"
#include "PixelShader.h"

class UIManager;
class HierarchyScreen : public AUIScreen
{
private:
	HierarchyScreen();
	~HierarchyScreen();

	virtual void drawUI() override;
	friend class UIManager;

public:
	static bool isOpen;
};


