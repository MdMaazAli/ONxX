#include "imgui.h"
#include "UIManager.h"
#include <filesystem>
#include <string>
#include <vector>

class FileManager{
    public:
        void DrawDirectoryNode (const std::filesystem::path& directoryPath){
            try{
                std::vector<std::filesystem::directory_entry> subDirectories;
                std::vector<std::filesystem::directory_entry> files;

                for(const auto& entry: std::filesystem::directory_iterator(directoryPath)){
                    if (entry.is_directory()) {
                        subDirectories.push_back(entry);
                    }
                    else{
                        files.push_back(entry);
                    }
                }
                
                for(const auto& dir: subDirectories){
                    std::string folderName = dir.path().filename().string();

                    ImGui::PushID(dir.path().string().c_str());

                    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow;
                    bool isOpen = ImGui::TreeNodeEx(folderName.c_str(), nodeFlags);

                    if (isOpen) {
                        DrawDirectoryNode(dir.path());
                        ImGui::TreePop();
                    }

                    ImGui::PopID();
                }

                for(const auto& file: files){
                    std::string fileName = file.path().filename().string();

                    ImGui::PushID(file.path().string().c_str());

                    ImGuiTreeNodeFlags fileFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
                    ImGui::TreeNodeEx(fileName.c_str(), fileFlags);

                    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                        m_hasDoubleClicked = true;
                        m_selectedPath = file.path().string();
                    }

                    // Drag & Drop into 3D Viewport or Hierarchy
                    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
                        ImGui::SetDragDropPayload("ASSET_FILE", file.path().string().c_str(), file.path().string().size() + 1);
                        ImGui::Text("Attach %s", fileName.c_str());
                        ImGui::EndDragDropSource();
                    }

                    ImGui::PopID();
                }
            }
            catch (std::filesystem::filesystem_error& e) {
                ImGui::TextColored(ImVec4(1.0f,1.0f,1.0f,1.0f), "Access Denied!!");
            }
        }

        bool hasDoubleClicked(){
            return m_hasDoubleClicked;
        }

        std::string ConsumeDoubleClickedPath(){
            m_hasDoubleClicked = false;
            return m_selectedPath;
        }
    
    private:
        bool m_hasDoubleClicked = false;
        std::string m_selectedPath = "";
};