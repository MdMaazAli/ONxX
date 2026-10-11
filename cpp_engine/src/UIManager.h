#pragma once

#include "imgui.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "Physics.h"

enum class EngineState { EDIT_MODE, TRAINING_MODE, INFERENCE_MODE };

class UIManager {
public:
    UIManager();
    ~UIManager();

    void Init();
    void Render(GLFWwindow* window, 
                GLuint fboTextureId, 
                std::vector<TransformComponent>& transforms, 
                const std::vector<PhysicsComponent>& physics, 
                std::vector<AgentComponent>& agents,
                float framerate,
                bool& cursorInViewport,
                const glm::mat4& view,
                const glm::mat4& projection,
                int activeEntityID);

    EngineState GetState() const { return m_state; }

    // Add a getter that main.cpp can call to check for 2D clicks
    int ConsumeHierarchySelection() { 
        int id = m_newlySelectedHierarchyID; 
        m_newlySelectedHierarchyID = -1; // Reset it immediately so it only fires once
        return id; 
    }
    
    // --- [NEW] Drag & Drop Event Outputs for main.cpp ---
    bool HasNewDrop() const { return m_hasNewDrop; }
    std::string GetDropPayload() const { return m_dropPayload; }
    glm::vec2 GetDropNDC() const { return m_dropNDC; }

    // --- [NEW] Mouse Click Event Outputs for main.cpp ---
    bool HasNewClick() const { return m_hasNewClick; }
    glm::vec2 GetClickNDC() const { return m_clickNDC; }
    bool HasNewRightClick() const { return m_hasNewRightClick; }
    glm::vec2 GetRightClickNDC() const { return m_rightClickNDC; }

    bool IsMouseDragging() const { return m_isMouseDragging; }
    glm::vec2 GetDragNDC() const { return m_dragNDC; }
    bool HasMouseReleased() const { return m_hasMouseReleased; }
    
    // --- [NEW] Viewport Popup trigger ---
    void OpenEntityPopup(int entityID) { m_popupEntityID = entityID; }

private:
    void SetupDockingLayout();
    void ApplyRetroHardwareTheme();
    void GenerateProceduralMetalTexture();
    void DrawWindowBackground(GLuint textureID);
    
    bool m_firstFrame;
    GLuint m_metalTexture;
    EngineState m_state;

    int m_newlySelectedHierarchyID = -1;
    
    // Drop State
    bool m_hasNewDrop = false;
    std::string m_dropPayload = "";
    glm::vec2 m_dropNDC = glm::vec2(0.0f);

    // Click State
    bool m_hasNewClick = false;
    glm::vec2 m_clickNDC = glm::vec2(0.0f);
    bool m_hasNewRightClick = false;
    glm::vec2 m_rightClickNDC = glm::vec2(0.0f);
    
    // Drag State
    bool m_isMouseDragging = false;
    glm::vec2 m_dragNDC = glm::vec2(0.0f);
    bool m_hasMouseReleased = false;
    
    // Popup State
    int m_popupEntityID = -1;
};
