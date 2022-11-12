#include "HierarchyScreen.h"
#include "imgui.h"
#include "UIManager.h"
#include <iostream>
#include "GameObjectManager.h"

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
        static float x, y, z;
        
        //Outline Selection
        ImGui::SetNextWindowSize(ImVec2(315, 200));
        ImGui::Begin("Hierarchy", &isOpen, ImGuiWindowFlags_NoResize);
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();

        for (int i = 0; i < GameObjectManager::get()->objList.size(); i++) {
            string str = "Select " + to_string(i);
            const char* selectLabel = str.data();
            if (ImGui::Button(selectLabel)) {
                GameObjectManager::get()->selectedObjList[i] = true;
            }
            ImGui::SameLine();
            str = "Unselect " + to_string(i);
            const char* unselectLabel = str.data();
            if (ImGui::Button(unselectLabel)) {
                GameObjectManager::get()->selectedObjList[i] = false;
            }          
            ImGui::SameLine();
            str = "Object " + to_string(i) + " Properties";
            const char* objLabel = str.data();
            if (ImGui::Button(objLabel)) {
                index = i;
                x = GameObjectManager::get()->objList[index]->getLocalPosition().m_x;
                y = GameObjectManager::get()->objList[index]->getLocalPosition().m_y;
                z = GameObjectManager::get()->objList[index]->getLocalPosition().m_z;
            }
        }
        ImGui::End();

        //Inspector
        if (!GameObjectManager::get()->objList.empty()) {
            ImGui::SetNextWindowSize(ImVec2(130, 200));
            ImGui::Begin("Inspector", &isOpen, ImGuiWindowFlags_NoResize);
            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Text("Position: ");
            ImGui::SliderFloat("X", &x, -10.0f, 10.0f);
            ImGui::SliderFloat("Y", &y, -10.0f, 10.0f);
            ImGui::SliderFloat("Z", &z, -10.0f, 10.0f);
            GameObjectManager::get()->changePosition(index, x, y, z);
            ImGui::End();
        }   

        //Saved Merge Actors
        if (!GameObjectManager::get()->mergedObjList.empty()) {
            ImGui::SetNextWindowSize(ImVec2(170, 200));
            ImGui::Begin("Merged Actors", &isOpen, ImGuiWindowFlags_NoResize);
            for (int i = 0; i < GameObjectManager::get()->mergedObjList.size(); i++) {
                String str = "Create Merged Actor " + to_string(i);
                const char* mALabel = str.data();
                if (ImGui::Button(mALabel)) {
                    GameObjectManager::get()->instantiateMergedActor(i);
                }
            }
            ImGui::End();
        }
    }
}
