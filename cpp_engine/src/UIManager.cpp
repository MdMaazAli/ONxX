#include "UIManager.h"
#include "imgui_internal.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include "ImGuizmo.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <fstream>

#if defined(_WIN32)
#include <windows.h>
#include <shellapi.h>
#endif
#include <cstdlib>
#include <filesystem>
#include <iostream>

// < --------------- SCRIPT ATTACHER CROSS PLATFORM CHECKER --------------- >
void LaunchScriptInEditor(const std::string& relativePath) {
    if (relativePath.empty()) return;

    // Resolve relative path to absolute so the OS knows exactly where it is
    std::filesystem::path fullPath = std::filesystem::absolute(relativePath);
    std::string pathStr = fullPath.string();

#if defined(_WIN32)
    // Try opening with the computer's registered "edit" application (VS Code, PyCharm, Notepad++, etc.)
    HINSTANCE result = ShellExecuteA(NULL, "edit", pathStr.c_str(), NULL, NULL, SW_SHOWNORMAL);
    
    // If no "edit" action is registered (error code <= 32), try the standard "open"
    if ((INT_PTR)result <= 32) {
        result = ShellExecuteA(NULL, "open", pathStr.c_str(), NULL, NULL, SW_SHOWNORMAL);
    }
    
    // Guaranteed fallback: If Windows has zero editor associated with .py, open with notepad
    if ((INT_PTR)result <= 32) {
        std::string fallbackCmd = "notepad.exe \"" + pathStr + "\"";
        system(fallbackCmd.c_str());
    }
#elif defined(__APPLE__)
    // macOS default text editor
    std::string cmd = "open -t \"" + pathStr + "\"";
    system(cmd.c_str());
#else
    // Linux default desktop text editor
    std::string cmd = "xdg-open \"" + pathStr + "\" &";
    system(cmd.c_str());
#endif
}

//  < ---------- PYTHON SCRIPT FOLDER CHECK ---------- >
void EnsureScriptExists(const std::string& relativePath, const std::string& modelType, int entityID) {
    std::filesystem::path p(relativePath);
    
    // Create the python_api directory if it doesn't exist on this machine
    if (p.has_parent_path()) {
        std::filesystem::create_directories(p.parent_path());
    }
    // Create the starter script template
    if (!std::filesystem::exists(p)) {
        std::ofstream file(p);
        if (file.is_open()) {
            file << "# ==========================================================\n";
            file << "# " << modelType << " Script for Entity [" << entityID << "]\n";
            file << "# Connected to C++ Engine via FlatBuffers / ZeroMQ\n";
            file << "# ==========================================================\n\n";
            file << "import sys\nimport os\n\n";
            file << "class " << (modelType == "PPO Trainer" ? "PPOAgent" : "DQNAgent") << ":\n";
            file << "    def __init__(self):\n";
            file << "        self.model_type = \"" << modelType << "\"\n\n";
            file << "    def act(self, obs):\n";
            file << "        # 0 = Move Forward, 1 = Jump, 2 = Move Sideways\n";
            file << "        return 0\n\n";
            file << "def step(obs):\n";
            file << "    return 0\n";
            file.close();
        }
    }
}

void SetRigidbody(PhysicsComponent& phyComp) {
    phyComp.Rigidbody = Rigidbody::YES;
}

void RemoveRigidbody(PhysicsComponent& phyComp) {
    phyComp.Rigidbody = Rigidbody::NO;
}

void SetAgentBrain(AgentComponent& agent, int entityID, const std::string& modelType) {
    agent.Mode = AgentMode::TRAINING;
    agent.AgentModule = modelType;

    if (modelType == "PPO Trainer") {
        agent.ScriptPath = "python_api/ppo_agent_" + std::to_string(entityID) + ".py";
    } else {
        agent.ScriptPath = "python_api/dqn_agent_" + std::to_string(entityID) + ".py";
    }

    // Auto-create the folder and starter script if they don't exist yet
    EnsureScriptExists(agent.ScriptPath, modelType, entityID);
}

void RemoveAgentBrain(AgentComponent& agent) {
    agent.Mode = AgentMode::NONE;
    agent.AgentModule = "None";
    agent.ScriptPath = "";
}

UIManager::UIManager() : m_firstFrame(true), m_metalTexture(0), m_state(EngineState::EDIT_MODE) {}

UIManager::~UIManager() {}

void UIManager::GenerateProceduralMetalTexture() {
    int width = 512;
    int height = 512;
    std::vector<unsigned char> data(width * height * 4);
    
    // Generate base noise
    for (int i = 0; i < width * height; ++i) {
        unsigned char noise = 40 + (rand() % 25); // Dark grey noise
        data[i*4 + 0] = noise;
        data[i*4 + 1] = noise;
        data[i*4 + 2] = noise;
        data[i*4 + 3] = 255;
    }
    
    glGenTextures(1, &m_metalTexture);
    glBindTexture(GL_TEXTURE_2D, m_metalTexture);
    
    // Set wrapping to GL_REPEAT so it tiles seamlessly across large windows
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
}

void UIManager::Init() {
    GenerateProceduralMetalTexture();
    ApplyRetroHardwareTheme();
}

void UIManager::DrawWindowBackground(GLuint textureID) {
    if (textureID == 0) return;
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 p_min = ImGui::GetWindowPos();
    ImVec2 size = ImGui::GetWindowSize();
    
    // Manually tile the texture in 512x512 chunks to bypass OpenGL sampler clamp limits
    float tileSize = 512.0f;
    for (float y = 0; y < size.y; y += tileSize) {
        for (float x = 0; x < size.x; x += tileSize) {
            float draw_w = (x + tileSize > size.x) ? (size.x - x) : tileSize;
            float draw_h = (y + tileSize > size.y) ? (size.y - y) : tileSize;
            
            ImVec2 tile_min = ImVec2(p_min.x + x, p_min.y + y);
            ImVec2 tile_max = ImVec2(tile_min.x + draw_w, tile_min.y + draw_h);
            
            ImVec2 uv_min = ImVec2(0.0f, 0.0f);
            ImVec2 uv_max = ImVec2(draw_w / tileSize, draw_h / tileSize);
            
            draw_list->AddImage((void*)(intptr_t)textureID, tile_min, tile_max, uv_min, uv_max, IM_COL32(200, 210, 200, 255));
        }
    }
}

void UIManager::ApplyRetroHardwareTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    style.WindowRounding = 0.0f;
    style.FrameRounding = 0.0f;
    style.GrabRounding = 0.0f;
    style.WindowBorderSize = 2.0f;
    style.FrameBorderSize = 2.0f;
    style.ItemSpacing = ImVec2(10.0f, 8.0f);
    style.ScrollbarRounding = 0.0f;

    colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    colors[ImGuiCol_ChildBg]  = ImVec4(0.08f, 0.08f, 0.08f, 0.7f);
    colors[ImGuiCol_PopupBg]  = ImVec4(0.10f, 0.10f, 0.10f, 0.9f);
    colors[ImGuiCol_Border]               = ImVec4(0.35f, 0.40f, 0.35f, 1.0f); 
    colors[ImGuiCol_BorderShadow]         = ImVec4(0.00f, 0.00f, 0.00f, 1.0f);
    colors[ImGuiCol_FrameBg]              = ImVec4(0.12f, 0.12f, 0.12f, 0.8f);
    colors[ImGuiCol_FrameBgHovered]       = ImVec4(0.18f, 0.25f, 0.18f, 0.9f);
    colors[ImGuiCol_FrameBgActive]        = ImVec4(0.15f, 0.35f, 0.15f, 0.9f);
    colors[ImGuiCol_TitleBg]              = ImVec4(0.10f, 0.10f, 0.10f, 0.8f);
    colors[ImGuiCol_TitleBgActive]        = ImVec4(0.15f, 0.22f, 0.15f, 0.8f); 
    colors[ImGuiCol_TitleBgCollapsed]     = ImVec4(0.05f, 0.05f, 0.05f, 0.8f);
    colors[ImGuiCol_Header]               = ImVec4(0.15f, 0.18f, 0.15f, 0.8f);
    colors[ImGuiCol_HeaderHovered]        = ImVec4(0.20f, 0.28f, 0.20f, 0.9f);
    colors[ImGuiCol_HeaderActive]         = ImVec4(0.25f, 0.35f, 0.25f, 0.9f);
    colors[ImGuiCol_Button]               = ImVec4(0.15f, 0.15f, 0.15f, 0.8f);
    colors[ImGuiCol_ButtonHovered]        = ImVec4(0.25f, 0.35f, 0.25f, 0.9f); 
    colors[ImGuiCol_ButtonActive]         = ImVec4(0.10f, 0.45f, 0.10f, 0.9f); 
    colors[ImGuiCol_Text]                 = ImVec4(0.4f, 1.0f, 0.6f, 1.0f);
    colors[ImGuiCol_TextDisabled]         = ImVec4(0.2f, 0.5f, 0.3f, 1.0f);
    colors[ImGuiCol_ScrollbarBg]          = ImVec4(0.05f, 0.05f, 0.05f, 0.5f);
    colors[ImGuiCol_ScrollbarGrab]        = ImVec4(0.20f, 0.20f, 0.20f, 0.8f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.30f, 0.35f, 0.30f, 0.9f);
    colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.15f, 0.45f, 0.15f, 0.9f);
}

void UIManager::SetupDockingLayout() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGuiID dockspace_id = ImGui::GetID("ONxX_DockSpace");

    ImGui::DockBuilderRemoveNode(dockspace_id);
    ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->WorkSize);

    ImGuiID right_id;
    ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.25f, &right_id, &dockspace_id);
    ImGuiID top_id;
    ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Up, 0.08f, &top_id, &dockspace_id);
    ImGuiID bottom_id;
    ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.25f, &bottom_id, &dockspace_id);
    ImGuiID left_id;
    ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.08f, &left_id, &dockspace_id);

    ImGuiID left_bottom_id;
    ImGui::DockBuilderSplitNode(left_id, ImGuiDir_Down, 0.4f, &left_bottom_id, &left_id);

    ImGui::DockBuilderDockWindow("Telemetry", right_id);
    ImGui::DockBuilderDockWindow("Top Bar", top_id);
    ImGui::DockBuilderDockWindow("Console", bottom_id);
    ImGui::DockBuilderDockWindow("Hierarchy", left_id);
    ImGui::DockBuilderDockWindow("Model Library", left_bottom_id);
    ImGui::DockBuilderDockWindow("Viewport", dockspace_id);

    ImGui::DockBuilderFinish(dockspace_id);
}

void UIManager::Render(GLFWwindow* window, GLuint fboTextureId, std::vector<TransformComponent>& transforms, std::vector<PhysicsComponent>& physics, std::vector<AgentComponent>& agents, float framerate, bool& cursorInViewport, const glm::mat4& view, const glm::mat4& projection, int activeEntityID) 
{
    ImGuizmo::BeginFrame();

    m_hasNewDrop = false; // Reset event every frame
    m_hasNewClick = false; // Reset click every frame
    m_hasNewRightClick = false; // Reset right-click every frame
    m_hasMouseReleased = false; // Reset release every frame
    if (m_firstFrame) {
        SetupDockingLayout();
        m_firstFrame = false;
    }

    ImGuiID dockspace_id = ImGui::GetID("ONxX_DockSpace");
    ImGui::DockSpaceOverViewport(dockspace_id, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

    ImGui::Begin("Telemetry", nullptr, ImGuiWindowFlags_NoCollapse);
    DrawWindowBackground(m_metalTexture);
    ImGui::Text("=== TELEMETRY ===");
    ImGui::Separator();
    if (!transforms.empty()) {
        ImGui::Text("POS X: %05.2f", transforms[0].Position.x);
        ImGui::Text("POS Y: %05.2f", transforms[0].Position.y);
        ImGui::Text("POS Z: %05.2f", transforms[0].Position.z);
    }
    ImGui::Separator();
    if (!physics.empty()) {
        ImGui::Text("VEL Y: %05.2f", physics[0].Velocity.y);
    }
    ImGui::Separator();
    ImGui::Text("FPS: %.1f", framerate);
    ImGui::End();

    // ------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------

    ImGui::Begin("Top Bar", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
    DrawWindowBackground(m_metalTexture);
    
    // --- MODE SWITCHER ---
    ImGui::Text("MODE:");
    ImGui::SameLine();
    
    // Helper lambda to draw a glowing button
    auto DrawModeButton = [&](const char* label, EngineState targetState, ImVec4 glowColor) {
        bool isActive = (m_state == targetState);
        if (isActive) {
            ImGui::PushStyleColor(ImGuiCol_Button, glowColor);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, glowColor);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, glowColor);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f)); // Black text on glow
            if (ImGui::Button(label)) m_state = targetState;
            ImGui::PopStyleColor(4);
        } else {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.1f, 0.1f, 1.0f)); // Dim
            if (ImGui::Button(label)) m_state = targetState;
            ImGui::PopStyleColor(1);
        }
    };

    DrawModeButton("[ EDIT ]", EngineState::EDIT_MODE, ImVec4(0.3f, 1.0f, 0.3f, 1.0f)); // Neon Green
    ImGui::SameLine();
    DrawModeButton("[ TRAIN ]", EngineState::TRAINING_MODE, ImVec4(1.0f, 0.2f, 0.2f, 1.0f)); // Neon Red
    ImGui::SameLine();
    DrawModeButton("[ INFERENCE ]", EngineState::INFERENCE_MODE, ImVec4(0.2f, 0.6f, 1.0f, 1.0f)); // Neon Blue
    
    ImGui::SameLine(ImGui::GetWindowWidth() - 150); // Push to far right
    if (ImGui::Button("EMERGENCY STOP")) {
        glfwSetWindowShouldClose(window, true);
    }
    ImGui::End();

    ImGui::Begin("Console", nullptr, ImGuiWindowFlags_NoCollapse);
    DrawWindowBackground(m_metalTexture);
    
    // --- Engraved Watermark ---
    ImDrawList* console_draw = ImGui::GetWindowDrawList();
    ImVec2 c_pos = ImGui::GetWindowPos();
    ImVec2 c_size = ImGui::GetWindowSize();
    const char* watermark = "ONxX";
    ImVec2 t_size = ImGui::CalcTextSize(watermark);
    float scale = 8.0f;
    ImVec2 center_pos = ImVec2(c_pos.x + (c_size.x - t_size.x * scale) * 0.5f, c_pos.y + (c_size.y - t_size.y * scale) * 0.5f);
    
    // Highlight (simulates light catching the bottom edge of an indentation)
    console_draw->AddText(ImGui::GetFont(), ImGui::GetFontSize() * scale, ImVec2(center_pos.x + 3.0f, center_pos.y + 3.0f), IM_COL32(100, 110, 100, 80), watermark);
    // Deep shadow (simulates the depth of the stamp)
    console_draw->AddText(ImGui::GetFont(), ImGui::GetFontSize() * scale, center_pos, IM_COL32(15, 20, 15, 200), watermark);

    // ------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------
    
    ImGui::Text("Initializing core modules...");
    ImGui::Text("ZMQ Listener active on port 5555.");
    ImGui::Text("Awaiting neural net commands...");
    ImGui::End();

    ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoCollapse);
    DrawWindowBackground(m_metalTexture);
    
    ImGui::Text("SCENE ENTITIES");
    ImGui::Separator();
    
    for (size_t i = 0; i < transforms.size(); ++i) {
        ImGui::PushID((int)i);
        
        char label[64];
        if (agents.size() > i && agents[i].AgentModule != "None") {
            sprintf(label, "[%zu] Entity (Brain: %s)", i, agents[i].AgentModule.c_str());
        } else {
            sprintf(label, "[%zu] Entity (Empty)", i);
        }
        
        // Highlight the text if its ID matches the activeEntityID from main.cpp
        bool isSelected = (activeEntityID == (int)i); 
        
        // If the user clicks this text, it returns TRUE for exactly 1 frame
        if (ImGui::Selectable(label, isSelected)) {
            m_newlySelectedHierarchyID = (int)i; // Save this click to send to main.cpp!
        }
        
        // < --------------------- Right Click Context Menu for Hierarchy --------------------- >
        if (isSelected && ImGui::BeginPopupContextItem("HierarchyEntityMenu")) {
            if(physics.size() > i){
                PhysicsComponent& phyComp = physics[i];
                bool isRb = (phyComp.Rigidbody != Rigidbody::NO);

                if (!isRb) {
                    if (ImGui::MenuItem("Rigidbody")) {
                        SetRigidbody(phyComp);
                    }
                }
                else{
                    ImGui::Separator();
                    if (ImGui::MenuItem("Remove Rigidbody")) {
                        RemoveRigidbody(phyComp);
                    }
                }
            }
            if (agents.size() > i) {
                auto& agent = agents[i];
                bool isAgent = (agent.Mode != AgentMode::NONE);

                if (!isAgent) {
                    if (ImGui::MenuItem("Convert to Agent Entity (DQN Default)")) {
                        SetAgentBrain(agent, (int)i, "DQN Trainer");
                    }
                } else {
                    ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.5f, 1.0f), "Brain: %s", agent.AgentModule.c_str());
                    ImGui::TextDisabled("Script: %s", agent.ScriptPath.c_str());
                    ImGui::Separator();

                    if (ImGui::MenuItem("Edit Script")) {
                        LaunchScriptInEditor(agent.ScriptPath);
                    }
                    if (ImGui::BeginMenu("Change Model")) {
                        if (ImGui::MenuItem("DQN Trainer", nullptr, agent.AgentModule == "DQN Trainer")) {
                            SetAgentBrain(agent, (int)i, "DQN Trainer");
                        }
                        if (ImGui::MenuItem("PPO Trainer", nullptr, agent.AgentModule == "PPO Trainer")) {
                            SetAgentBrain(agent, (int)i, "PPO Trainer");
                        }
                        ImGui::EndMenu();
                    }
                    ImGui::Separator();
                    if (ImGui::MenuItem("Remove Agent Component")) {
                        RemoveAgentBrain(agent);
                    }
                }
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Delete")) {
                m_deleteRequestID = (int)i;
            }
            ImGui::EndPopup();
        }
        
        // Setup Drag and Drop Target for the Hierarchy Item
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RL_MODEL")) {
                const char* moduleName = (const char*)payload->Data;
                if (agents.size() > i) {
                    SetAgentBrain(agents[i], (int)i, moduleName);
                }
            }
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RL_BRAIN")) {
                const char* moduleName = (const char*)payload->Data;
                if (agents.size() > i) {
                    agents[i].Mode = AgentMode::INFERENCE;
                    agents[i].AgentModule = moduleName;
                }
            }
            ImGui::EndDragDropTarget();
        }
        
        ImGui::PopID();
    }
    ImGui::Dummy(ImVec2(0, 20));
    ImGui::Text("3D PRIMITIVES");
    ImGui::Separator();
    
    ImGui::Button("Spawn Sphere", ImVec2(-1, 30));
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
        const char* payload = "Primitive_Sphere";
        ImGui::SetDragDropPayload("3D_PRIMITIVE", payload, strlen(payload) + 1);
        ImGui::Text("Drop to spawn a new 3D object on the floor...");
        ImGui::EndDragDropSource();
    }
    
    ImGui::Button("Spawn Cube", ImVec2(-1, 30));
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
        const char* payload = "Primitive_Cube";
        ImGui::SetDragDropPayload("3D_PRIMITIVE", payload, strlen(payload) + 1);
        ImGui::Text("Drop to spawn a new 3D object on the floor...");
        ImGui::EndDragDropSource();
    }
    
    ImGui::Button("Spawn Pyramid", ImVec2(-1, 30));
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
        const char* payload = "Primitive_Pyramid";
        ImGui::SetDragDropPayload("3D_PRIMITIVE", payload, strlen(payload) + 1);
        ImGui::Text("Drop to spawn a new 3D object on the floor...");
        ImGui::EndDragDropSource();
    }

    ImGui::End();

    // ------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------

    ImGui::Begin("Model Library", nullptr, ImGuiWindowFlags_NoCollapse);
    DrawWindowBackground(m_metalTexture);
    
    ImGui::Text("MODEL SCRIPTS");
    ImGui::Separator();
    
    // Create draggable modules
    ImGui::Button("DQN Trainer", ImVec2(-1, 30));
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
        const char* payload = "DQN Trainer";
        ImGui::SetDragDropPayload("RL_MODEL", payload, strlen(payload) + 1);
        ImGui::Text("Attach DQN Trainer to Entity...");
        ImGui::EndDragDropSource();
    }
    
    ImGui::Button("PPO Trainer", ImVec2(-1, 30));
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
        const char* payload = "PPO Trainer";
        ImGui::SetDragDropPayload("RL_MODEL", payload, strlen(payload) + 1);
        ImGui::Text("Attach PPO Trainer to Entity...");
        ImGui::EndDragDropSource();
    }
    
    ImGui::Dummy(ImVec2(0, 20));
    ImGui::Text("EXTRACTED BRAINS");
    ImGui::Separator();
    
    ImGui::Button("DQN_Brain_v1.onnx", ImVec2(-1, 30));
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
        const char* payload = "DQN_Brain_v1";
        ImGui::SetDragDropPayload("RL_BRAIN", payload, strlen(payload) + 1);
        ImGui::Text("Plug DQN_Brain_v1 into Entity...");
        ImGui::EndDragDropSource();
    }
    
    ImGui::End();

    // ------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------

    ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoCollapse);
    ImVec2 availSpace = ImGui::GetContentRegionAvail();
    float targetAspect = 1920.0f / 1080.0f;
    float panelAspect = availSpace.x / availSpace.y;

    ImVec2 finalSize;
    if (panelAspect > targetAspect) {
        finalSize.y = availSpace.y;
        finalSize.x = availSpace.y * targetAspect;
    } else {
        finalSize.x = availSpace.x;
        finalSize.y = availSpace.x / targetAspect;
    }

    float cursorX = (availSpace.x - finalSize.x) * 0.5f;
    float cursorY = (availSpace.y - finalSize.y) * 0.5f;
    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + cursorX, ImGui::GetCursorPosY() + cursorY));

    ImGui::Image((void*)(intptr_t)fboTextureId, finalSize, ImVec2(0, 1), ImVec2(1, 0));
    cursorInViewport = ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);
    
    // --- [NEW] ImGuizmo Rendering ---
    ImVec2 p0 = ImGui::GetItemRectMin();
    ImVec2 p1 = ImGui::GetItemRectMax();
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();
    ImGuizmo::SetRect(p0.x, p0.y, p1.x - p0.x, p1.y - p0.y);
    
    if (activeEntityID != -1 && activeEntityID < transforms.size()) {
        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), transforms[activeEntityID].Position);
        
        ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection),
                             ImGuizmo::TRANSLATE, ImGuizmo::WORLD,
                             glm::value_ptr(modelMatrix));
                             
        // Update transforms if manipulated
        if (ImGuizmo::IsUsing()) {
            transforms[activeEntityID].Position = glm::vec3(modelMatrix[3]);
        }
    }

    // ------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------
    
    // Setup Drag and Drop Target on the Viewport
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RL_MODEL")) {
            const char* moduleName = (const char*)payload->Data;
            if(activeEntityID != -1 && activeEntityID < (int)agents.size()){
                SetAgentBrain(agents[activeEntityID], activeEntityID, moduleName);
            }
        }
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RL_BRAIN")) {
            const char* moduleName = (const char*)payload->Data;
            if (!agents.empty()) {
                agents[0].Mode = AgentMode::INFERENCE;
                agents[0].AgentModule = moduleName;
            }
        }
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("3D_PRIMITIVE")) {
            m_hasNewDrop = true;
            m_dropPayload = (const char*)payload->Data;
            
            ImVec2 mousePos = ImGui::GetMousePos();
            m_dropNDC.x = (2.0f * (mousePos.x - p0.x)) / finalSize.x - 1.0f;
            m_dropNDC.y = 1.0f - (2.0f * (mousePos.y - p0.y)) / finalSize.y;
        }
        ImGui::EndDragDropTarget();
    }
    
    ImVec2 mousePos = ImGui::GetMousePos();
    
    auto calcNDC = [&]() {
        return glm::vec2(
            (2.0f * (mousePos.x - p0.x)) / finalSize.x - 1.0f,
            1.0f - (2.0f * (mousePos.y - p0.y)) / finalSize.y
        );
    };

    if (ImGui::IsWindowHovered()) {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            if (!ImGuizmo::IsOver()) {
                m_hasNewClick = true;
                m_clickNDC = calcNDC();
            }
        }
        
        // Track Right Clicks for Context Menu
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
            if (!ImGuizmo::IsOver()) {
                m_hasNewRightClick = true;
                m_rightClickNDC = calcNDC();
            }
        }
        
        // Use IsMouseDown so we track it continuously while held
        if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
            if (!ImGuizmo::IsUsing()) {
                m_isMouseDragging = true;
                m_dragNDC = calcNDC();
            }
        }
    }
    
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        m_hasMouseReleased = true;
        m_isMouseDragging = false;
    }

    // ------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------

    static int activePopupID = -1;
    if (m_popupEntityID != -1) {
        activePopupID = m_popupEntityID;
        ImGui::OpenPopup("ViewportEntityMenu");
        m_popupEntityID = -1;
    }
    
    if (ImGui::BeginPopup("ViewportEntityMenu")) {
        ImGui::Text("Entity ID: %d", activePopupID);
        ImGui::Separator();
        if (physics.size() > (size_t)activePopupID && activePopupID >= 0) {
            PhysicsComponent& phyComp = physics[activePopupID];
            bool isRb = (phyComp.Rigidbody != Rigidbody::NO);

            if (!isRb) {
                if (ImGui::MenuItem("Rigidbody")) {
                    SetRigidbody(phyComp);
                }
            } else {
                ImGui::Separator();
                if (ImGui::MenuItem("Remove Rigidbody")) {
                    RemoveRigidbody(phyComp);
                }
            }
            ImGui::Separator();
        }
        if (agents.size() > (size_t)activePopupID && activePopupID >= 0) {
            auto& agent = agents[activePopupID];
            bool isAgent = (agent.Mode != AgentMode::NONE);

            if (!isAgent) {
                if (ImGui::MenuItem("Convert to Agent Entity (DQN Default)")) {
                    SetAgentBrain(agent, activePopupID, "DQN Trainer");
                }
            } else {
                ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.5f, 1.0f), "Brain: %s", agent.AgentModule.c_str());
                ImGui::TextDisabled("Script: %s", agent.ScriptPath.c_str());
                ImGui::Separator();

                if (ImGui::MenuItem("Edit Script")) {
                    LaunchScriptInEditor(agent.ScriptPath);
                }
                if (ImGui::BeginMenu("Change Model")) {
                    if (ImGui::MenuItem("DQN Trainer", nullptr, agent.AgentModule == "DQN Trainer")) {
                        SetAgentBrain(agent, activePopupID, "DQN Trainer");
                    }
                    if (ImGui::MenuItem("PPO Trainer", nullptr, agent.AgentModule == "PPO Trainer")) {
                        SetAgentBrain(agent, activePopupID, "PPO Trainer");
                    }
                    ImGui::EndMenu();
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Remove Agent Component")) {
                    RemoveAgentBrain(agent);
                }
            }
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Delete")) {
            m_deleteRequestID = activePopupID;
        }
        ImGui::EndPopup();
    }
    
    ImGui::End();
}
