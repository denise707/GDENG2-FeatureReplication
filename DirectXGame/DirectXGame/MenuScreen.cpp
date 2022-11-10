#include "MenuScreen.h"
#include "imgui.h"
#include "UIManager.h"
#include "HierarchyScreen.h"

MenuScreen::MenuScreen() : AUIScreen("MenuScreen")
{

}

MenuScreen::~MenuScreen()
{
    AUIScreen::~AUIScreen();
}

void MenuScreen::drawUI()
{
    bool* p_open = new bool;
    *p_open = true;
    ImGui::SetNextWindowSize(ImVec2(UIManager::WINDOW_WIDTH, UIManager::WINDOW_HEIGHT));
	if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Tools"))
        {
            if (ImGui::MenuItem("Outline Selected..")) { HierarchyScreen::isOpen = true; }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}
