#pragma once

#include <iostream>
#include "imgui.h"
#include <filesystem>
#include <string>
#include <vector>
#include <fstream>

class FileManager{
    public:
        void DrawDirectoryNode (const std::filesystem::path& directoryPath, bool isRoot = true){
            try{
                std::string pathStr = directoryPath.string();
                if (isRoot) {
                    if(ImGui::BeginPopupContextWindow("Menu")){
                        if (ImGui::MenuItem("Create Folder")) {
                            int count = 1;
                            std::string folderName = "New Folder";

                            while(std::filesystem::exists(pathStr+"/"+folderName)){
                                folderName ="New Folder ("+std::to_string(count)+")";
                                count++;
                            }

                            if (std::filesystem::create_directories(pathStr+"/"+folderName)) {
                                std::cout<<"[CREATED] NEW FOLDER AT : "<<pathStr<<std::endl;
                            }
                        }
                        if (ImGui::BeginMenu("Create File")) {
                            if (ImGui::MenuItem("Create .py")) std::ofstream(GetUniquePath(pathStr, "New File", ".py"));
                            if (ImGui::MenuItem("Create .cpp")) std::ofstream(GetUniquePath(pathStr, "New File", ".cpp"));
                            if (ImGui::MenuItem("Create .c")) std::ofstream(GetUniquePath(pathStr, "New File", ".c"));
                            if (ImGui::MenuItem("Create Notebook")) std::ofstream(GetUniquePath(pathStr, "New File", ".ipynb"));

                            ImGui::EndMenu();
                        }
                        ImGui::EndPopup();
                    }
                }
                
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
                    std::string subDirName = dir.path().filename().string();
                    std::string subPathStr = dir.path().string();
                    
                    ImGui::PushID(dir.path().string().c_str());
                    
                    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow;
                    bool isOpen = ImGui::TreeNodeEx(subDirName.c_str(), nodeFlags);
                    
                    if (ImGui::BeginPopupContextItem("Sub Folder Directory")) {
                        if (ImGui::MenuItem("Create Folder")) {
                            int count = 1;
                            std::string folderName = "New Folder";
                            
                            while(std::filesystem::exists(subPathStr+"/"+folderName)){
                                folderName ="New Folder ("+std::to_string(count)+")";
                                count++;
                            }
                            
                            if (std::filesystem::create_directories(subPathStr+"/"+folderName)) {
                                std::cout<<"[CREATED] NEW FOLDER AT : "<<subPathStr<<std::endl;
                            }
                        }
                        if (ImGui::BeginMenu("Create File")){
                            if (ImGui::MenuItem("Create .py")) std::ofstream(GetUniquePath(subPathStr, "New File", ".py"));
                            if (ImGui::MenuItem("Create .cpp")) std::ofstream(GetUniquePath(subPathStr, "New File", ".cpp"));
                            if (ImGui::MenuItem("Create .c")) std::ofstream(GetUniquePath(subPathStr, "New File", ".c"));
                            if (ImGui::MenuItem("Create Notebook")) std::ofstream(GetUniquePath(subPathStr, "New File", ".ipynb"));
                            
                            ImGui::EndMenu();
                        }
                        ImGui::EndPopup();
                    }

                    if (isOpen) {
                        DrawDirectoryNode(dir.path(), false);
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

        std::string GetUniquePath(std::string directory, std::string basename, std::string extension){
            std::string fullPath = directory+"/"+basename+extension;
            if (!std::filesystem::exists(directory+"/"+basename+extension)) return fullPath;
            int count = 1;
            
            while(true){
                fullPath = directory+"/"+basename+" ("+std::to_string(count)+")"+extension;
                if (!std::filesystem::exists(fullPath)) return fullPath;
                count++;
            }
        }
    
    private:
        bool m_hasDoubleClicked = false;
        std::string m_selectedPath = "";
};