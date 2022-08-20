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

        CameraControl();

        ActorControls();

        float fps = ImGui::GetIO().Framerate;
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / fps, fps);
        ImGui::End();

    }

    void AppUI::CameraControl() {
        Renderer &renderer = _engine.renderer();
        if (ImGui::TreeNode("Camera")) {
            ImGui::DragFloat("fov", &renderer.camera()->fov, 0.01f);
            ImGui::DragFloat("aspect", &renderer.camera()->aspect, 0.01f);
            ImGui::DragFloat("near", &renderer.camera()->near, 0.01f);
            ImGui::DragFloat("far", &renderer.camera()->far, 0.01f);
            ImGui::DragScalarN("position", ImGuiDataType_Float,&renderer.camera()->transform.position, 3, 0.01f);
            ImGui::DragScalarN("front", ImGuiDataType_Float,&renderer.camera()->front, 3, 0.01f);
            ImGui::DragScalarN("up", ImGuiDataType_Float,&renderer.camera()->up, 3, 0.01f);
            ImGui::Separator();
            ImGui::TreePop();
        }
    }

    void AppUI::ActorControls() {
        auto &meshActors = _engine.renderer().meshActors();
        if (ImGui::TreeNode("Actors")) {
            for (int i = 0; i < meshActors.size(); i++) {
                auto &actor = meshActors[i];
                ImGui::PushID(i);
                if (ImGui::TreeNode("Actor %s", actor->name.c_str())) {
                    ImGui::Text("transform");
                    ImGui::DragScalarN("position", ImGuiDataType_Float, &actor->transform.position, 3, 0.01f);
                    ImGui::DragScalarN("rotation", ImGuiDataType_Float, &actor->transform.rotation, 3, 0.1f);
                    ImGui::DragScalarN("scale", ImGuiDataType_Float, &actor->transform.scale, 3, 0.1f);
                    ImGui::Separator();
                    ImGui::TreePop();
                }

                ImGui::PopID();
            }
            ImGui::TreePop();
        }
    }

    AppUI::AppUI(VulkanEngine &vulkanEngine) : _engine(vulkanEngine) {}
} // Enix