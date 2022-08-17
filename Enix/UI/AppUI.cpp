//
// Created by Energy on 2022/08/18.
//

#include "AppUI.h"
#include "imgui.h"
#include "VulkanEngine.h"

namespace Enix {
    void AppUI::onDraw() {
        ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), dockspaceFlags);
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New")) {
                }
                if (ImGui::MenuItem("Open", "Ctrl+O")) {
                }
                if (ImGui::MenuItem("Save", "Ctrl+S")) {
                }
                if (ImGui::MenuItem("Save As..")) {
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit")) {
                if (ImGui::MenuItem("Undo", "CTRL+Z")) {
                }
                if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {
                } // Disabled item
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "CTRL+X")) {
                }
                if (ImGui::MenuItem("Copy", "CTRL+C")) {
                }
                if (ImGui::MenuItem("Paste", "CTRL+V")) {
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View")) {
                if (ImGui::MenuItem("ShowDemo", "F12")) {
                   _showDemo = true;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        if (_showDemo) {
            ImGui::ShowDemoWindow(&_showDemo);
        }
        ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

        Renderer &renderer = _engine.renderer();

        ImGui::Text("Camera fov: %f degree", renderer.camera()->fov); // Display some text (you can use a format strings too)

        float fps = ImGui::GetIO().Framerate;
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / fps, fps);
        ImGui::End();

    }

    AppUI::AppUI(VulkanEngine &vulkanEngine) : _engine(vulkanEngine) {}
} // Enix