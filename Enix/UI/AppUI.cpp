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
            ImGui::DragFloat("transform.position.x", &renderer.camera()->transform.position.x, 0.01f);
            ImGui::DragFloat("transform.position.y", &renderer.camera()->transform.position.y, 0.01f);
            ImGui::DragFloat("transform.position.z", &renderer.camera()->transform.position.z, 0.01f);
            ImGui::DragFloat("front.x", &renderer.camera()->front.x, 0.01f);
            ImGui::DragFloat("front.y", &renderer.camera()->front.y, 0.01f);
            ImGui::DragFloat("front.z", &renderer.camera()->front.z, 0.01f);
            ImGui::DragFloat("up.x", &renderer.camera()->up.x, 0.01f);
            ImGui::DragFloat("up.y", &renderer.camera()->up.y, 0.01f);
            ImGui::DragFloat("up.z", &renderer.camera()->up.z, 0.01f);
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
                    ImGui::DragFloat("position.x", &actor->transform.position.x, 0.01f);
                    ImGui::DragFloat("position.y", &actor->transform.position.y, 0.01f);
                    ImGui::DragFloat("position.z", &actor->transform.position.z, 0.01f);
                    ImGui::DragFloat("rotation.x", &actor->transform.rotation.x, 0.1f);
                    ImGui::DragFloat("rotation.y", &actor->transform.rotation.y, 0.1f);
                    ImGui::DragFloat("rotation.z", &actor->transform.rotation.z, 0.1f);
                    ImGui::DragFloat("scale.x", &actor->transform.scale.x, 0.01f);
                    ImGui::DragFloat("scale.y", &actor->transform.scale.y, 0.01f);
                    ImGui::DragFloat("scale.z", &actor->transform.scale.z, 0.01f);
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