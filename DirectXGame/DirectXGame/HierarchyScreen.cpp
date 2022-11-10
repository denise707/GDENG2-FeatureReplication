#include "HierarchyScreen.h"
#include "imgui.h"
#include "UIManager.h"
#include <iostream>

bool HierarchyScreen::isOpen = false;

HierarchyScreen::HierarchyScreen() : AUIScreen("HierarchyScreen")
{

}

HierarchyScreen::~HierarchyScreen()
{
    AUIScreen::~AUIScreen();
}

void HierarchyScreen::drawUI()
{
    if (isOpen)
    {
        ImGui::SetNextWindowSize(ImVec2(200, 200));
        ImGui::Begin("Hierarchy", &isOpen, ImGuiWindowFlags_NoResize);
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();

        //0
        if (ImGui::Button("Select 0")) {
            AppWindow::selectedObjList[0] = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Unselect 0")) {
            AppWindow::selectedObjList[0] = false;
        }

        //1
        if (ImGui::Button("Select 1")) {
            AppWindow::selectedObjList[1] = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Unselect 1")) {
            AppWindow::selectedObjList[1] = false;
        }

        //2
        if (ImGui::Button("Select 2")) {
            AppWindow::selectedObjList[2] = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Unselect 2")) {
            AppWindow::selectedObjList[2] = false;
        }

        ImGui::End();
    }
}
