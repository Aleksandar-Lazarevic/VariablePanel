#include "VariablePanel.hpp"
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h> 
#include <optional>
#include <imgui/misc/cpp/imgui_stdlib.h>
#include <Widgets/imgui_widgets.h>
#include <Widgets/imgui_scopes.hpp>
#include "../Treeview/Node.hpp"
#include <stb/stb_image.h>
#include <glad/glad.h>
#include "../Treeview/VariableFolder.hpp"
#include "../Treeview/VariableNode.hpp"

using namespace panels;

#define GET_PARENT_EXCLUSICE(node)  (((treeview::VariableFolder*)node->getParent())->getExclusive())

VariablePanel::VariablePanel() :Layer("VariablePanel")
{
    m_selectedNode = nullptr;
    m_renameNode = nullptr;
    m_curHoverNode = nullptr;
    m_contextMenuNode = nullptr;

    m_selectedDataNode = nullptr;
    m_setDataNode = nullptr;

    m_srcMoveNode = nullptr;
    m_dstMoveNode = nullptr;

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    static ImWchar ranges[] = { 0xe000, 0xf8ff, 0 };
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    //icons_config.GlyphMinAdvanceX = iconFontSize;
    IconFont = io.Fonts->AddFontFromFileTTF("assets/fonts/MaterialIcons.ttf", 13.0f, &icons_config, ranges);
}

using namespace Hazel;

void VariablePanel::attachDataSource(std::shared_ptr<treeview::Node> newdata)
{
	m_treeRoot = std::move(newdata);
}

void VariablePanel::OnAttach()
{

}

void VariablePanel::OnDetach()
{
}

void VariablePanel::OnUpdate(Hazel::Timestep ts)
{
}

// Simple helper function to load an image into a OpenGL texture with common settings
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}
void VariablePanel::OnEvent(Hazel::Event& e)
{
}

void VariablePanel::OnImGuiRender()
{
    ImGui::SetNextWindowSizeConstraints(ImVec2(200, 200), ImVec2(FLT_MAX, FLT_MAX));
    auto variableWindow = imgui_scopes::Window::Begin("Variables", &m_guiState.m_show_window, ImGuiWindowFlags_NoCollapse);
    if (!variableWindow)
        return;
    if (!m_treeRoot)
    {
        ImGui::Text("No Object Selected");
        return;
    }
    const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
    const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
    static ImGuiTableFlags flags = ImGuiTableFlags_BordersInner | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable;
    if (ImGui::BeginTable("3ways", 3, flags))
    {
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_IndentDisable);
        ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_IndentEnable, TEXT_BASE_WIDTH * 12.0f);
        ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 18.0f);
        for (auto& it : *m_treeRoot)
        {
            DrawNode(it.get());
        }
        ImGui::EndTable();
    }

    
    if ((m_curHoverNode || m_contextMenuNode) && m_renameNode == nullptr)
    {
        if (ImGui::BeginPopupContextWindow())
        {
            if (!m_contextMenuNode)
            {
                m_contextMenuNode = m_curHoverNode;
                m_selectedNode = m_contextMenuNode;
            }
            DrawContextMenu();
            ImGui::EndPopup();
        }
        else
        {
            m_contextMenuNode = nullptr;
        }
    }
    
    m_curHoverNode = nullptr;

    if (m_srcMoveNode && m_dstMoveNode)
    {
        treeview::Node* srcParent = m_srcMoveNode->getParent();
        int nIdx = srcParent->find(*m_srcMoveNode);
        srcParent->move(nIdx, *(m_dstMoveNode->getParent()), m_dstMoveNode->getParent()->find(*m_dstMoveNode));
    }
    m_srcMoveNode = nullptr;
    m_dstMoveNode = nullptr;
}

void panels::VariablePanel::DrawNode(treeview::Node* node)
{
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    
    std::string strName = node->getName();
    const char* szNodeName = strName.c_str();
    treeview::VariableFolder* folder = nullptr;
    treeview::VariableNode* varNode = nullptr;
    variables::Value val = false;
    bool bParentExclusive = GET_PARENT_EXCLUSICE(node);
    bool bExclusive = false;
    bool bIsFolder = false;
    bool bEditing = false;
    treeview::ValueType valType = treeview::e_VT_Data;

    if (node->canBeParent())
    {
        bIsFolder = true;
        folder = (treeview::VariableFolder*)node;
        bExclusive = folder->getExclusive();
    }
    else
    {
        varNode = (treeview::VariableNode*)node;
        val = varNode->getValue();
        valType = varNode->GetValueType();
    }

    // Icon Draw..
    if (folder && folder->getExclusive())
    {
        ImGui::Text(u8"\uE627");
    }
    else if (varNode)
    {
        variables::Value val = varNode->getValue();
        if (val.index() == 3) //Option Variable
        {
            bool bActive = varNode->getVis();
            if (bActive) ImGui::Text(u8"\uE8F4");
            else ImGui::TextDisabled(u8"\uE835");;
        }
        else // Data variable
        {
            ImGui::Text(u8"\uE627");
        }

    }

    if (ImGui::IsItemClicked())
    {
        if (folder && folder->getExclusive())
        {
            ClickExclusiveIcon(node);
        }
        else if (varNode)
        {
            if (varNode->GetValueType() == treeview::e_VT_Exclusive)
            {
                ClickOptionValueIcon(node);
            }
            else
            {
                ClickDataValueIcon(node);
            }
        }
    }

    ImGui::TableNextColumn();
    bool bOpen = false;
    if (bIsFolder)
    {
        bOpen = ImGui::TreeNodeEx(node,
            ImGuiTreeNodeFlags_AllowItemOverlap |
            ImGuiTreeNodeFlags_SpanFullWidth |
            ImGuiTreeNodeFlags_DefaultOpen |
            ImGuiTreeNodeFlags_OpenOnArrow, 
            "");//node->Name
    }
    else
    {
        ImGui::TreeNodeEx(node, ImGuiTreeNodeFlags_Leaf | 
            ImGuiTreeNodeFlags_AllowItemOverlap |
            /*ImGuiTreeNodeFlags_Bullet |*/ 
            ImGuiTreeNodeFlags_NoTreePushOnOpen | 
            ImGuiTreeNodeFlags_SpanFullWidth, 
            "");
    }
    if (ImGui::IsItemHovered())
    {
        m_curHoverNode = node;        
    }
    
    if (ImGui::IsItemClicked())
    {
        if (m_selectedNode != node)
        {
            m_selectedNode = node;
            m_renameNode = nullptr;
        }
        else
        {
            m_renameNode = node;
        }
        if (!bIsFolder && valType == treeview::e_VT_Exclusive)
        {
            bool bActive = !varNode->getVis();
            varNode->setVis(bActive);
        }
    }
    ImGui::SameLine();
    ImGui::PushFont(IconFont);
    if (bIsFolder)
    {
        if (bExclusive)
        {
            ImGui::Text(u8"\uE896");
        }
        else
        {
            ImGui::Text(u8"\uE2C8");
        }
    }
    else
    {
        if (valType == treeview::e_VT_Exclusive)
        {
            bool bActive = varNode->getVis();
            char szBuff[1024];
            sprintf(szBuff, "##%s", szNodeName);
            if(bActive)
                ImGui::Text(u8"\uE837");
            else
                ImGui::Text(u8"\uE836");
            //if (ImGui::RadioButton(szBuff, bActive)) bActive = true;
            varNode->setVis(bActive);
        }
        else if (valType == treeview::e_VT_Bool)
        {
            ImGui::Text("{X}  ");
        }
        else
        {
            ImGui::Text("{d}  ");
        }
    }
    ImGui::PopFont();
    ImGui::SameLine();
    if (m_renameNode == node)
    {
        bEditing = true;
        const int BUFFER_SIZE = 1024; // or whatever size you need
        char myBuffer[BUFFER_SIZE]; // allocate memory dynamically
        strcpy(myBuffer, szNodeName);

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
        bool bRenamed = ImGui::InputText("###rename", &myBuffer[0], BUFFER_SIZE, ImGuiInputTextFlags_EnterReturnsTrue);
        ImGui::PopStyleVar(4);
        if (bRenamed)
        {
            m_renameNode = nullptr;
            node->rename(myBuffer);
        }
    }
    else
    {
        ImGui::Selectable(szNodeName, m_selectedNode == node);
        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("MY_MOVE_PAYLOAD", &node, sizeof(node));
            ImGui::Text(szNodeName);
            ImGui::EndDragDropSource();
        }
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MY_MOVE_PAYLOAD"))
            {
                IM_ASSERT(payload->DataSize == sizeof(node));
                treeview::Node* srcNode = *(treeview::Node**)payload->Data;
                m_srcMoveNode = srcNode;
                m_dstMoveNode = node;
            }
            ImGui::Separator();
            ImGui::Separator();
            ImGui::EndDragDropTarget();
        }
    }
    
    ImGui::TableNextColumn();
    if (folder)
    {
        if (bExclusive)
        {
            ImGui::Text(folder->getString().c_str());
        }
    }
    else
    {
        if (varNode->GetValueType() == treeview::e_VT_Data)
        {
            ImGui::Selectable("", m_selectedDataNode == node, ImGuiSelectableFlags_AllowItemOverlap);
            ImGui::SameLine();
            if (ImGui::IsItemClicked())
            {
                if (m_selectedDataNode != node)
                {
                    m_selectedDataNode = node;
                    m_setDataNode = nullptr;
                }
                else
                {
                    m_setDataNode = node;
                }
            }
            if (m_setDataNode == node)
            {
                bEditing = true;
                const int BUFFER_SIZE = 1024; // or whatever size you need
                char myBuffer[BUFFER_SIZE]; // allocate memory dynamically
                strcpy(myBuffer, varNode->getString().c_str());

                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
                bool bRenamed = ImGui::InputText("###rename", &myBuffer[0], BUFFER_SIZE, ImGuiInputTextFlags_EnterReturnsTrue);
                ImGui::PopStyleVar(4);
                if (bRenamed)
                {
                    m_setDataNode = nullptr;
                    varNode->setValue(myBuffer);
                }
            }
            else
            {
                ImGui::Text(varNode->getString().c_str());
            }
        }        
    }
    if (bOpen)
    {
        for (auto& it : *node)
        {
            DrawNode(it.get());
        }
        ImGui::TreePop();
    }
    
}

void panels::VariablePanel::DrawContextMenu()
{
    if (!m_contextMenuNode) return;
    treeview::Node* node = m_contextMenuNode;
    treeview::Node* parent = node->getParent();
    std::string strName = node->getName();
    std::string strNewName = strName + "_copy";

    const char* szNodeName = strName.c_str();
    treeview::VariableFolder* folder = nullptr;
    treeview::VariableNode* varNode = nullptr;
    if (node->canBeParent())
    {
        folder = (treeview::VariableFolder*)node;
    }
    else
    {
        varNode = (treeview::VariableNode*)node;
    }
    
    /*HZ_CORE_INFO("enable {0} {1} {2}", 
        (folder && !folder->getExclusive()), 
        (!folder && !GET_PARENT_EXCLUSICE(node)),
        (folder && !folder->getExclusive()) || (!folder && !GET_PARENT_EXCLUSICE(node))
    );*/
    if (ImGui::MenuItem("New Folder", NULL, false, 
        (!folder && !GET_PARENT_EXCLUSICE(node)) || (folder && !folder->getExclusive())))
    {
        if (folder)
        {
            if (auto* foldernode = folder->insertChild<treeview::VariableFolder>(0))
            {
                foldernode->rename(strNewName);
            }
        }
        else
        {
            int nIdx = parent->find(*node);
            if (auto* foldernode = node->getParent()->insertChild<treeview::VariableFolder>(nIdx + 1))
            {
                foldernode->rename(strNewName);
            }
        }
    }
    if (ImGui::MenuItem("New Exclusive Folder", NULL, false,
        folder && !folder->getExclusive()))
    {
        if (auto* foldernode = folder->insertChild<treeview::VariableFolder>(0))
        {
            foldernode->setExclusive(true);
            foldernode->rename(strNewName);
        }
    }
    ImGui::Separator();
    if (ImGui::MenuItem("New Option Variable", NULL, false, true))
    {
        if (folder)
        {
            if (auto* boolnode = folder->insertChild<treeview::VariableNode>(0, "New option Variable"))
                boolnode->setValue("false");
        }
        else
        {
            int nIdx = parent->find(*node);
            if (auto* boolnode = parent->insertChild<treeview::VariableNode>(nIdx + 1, strNewName))
                boolnode->setValue("false");
        }
    }
    if (ImGui::MenuItem("New Data Variable", NULL, false, 
        (folder && !folder->getExclusive()) || (!folder && !GET_PARENT_EXCLUSICE(node))))
    {
        if (folder)
        {
            if (auto* stringnode = folder->insertChild<treeview::VariableNode>(0, "New data Variable"))
                stringnode->setValue("enter text here");
        }
        else
        {
            int nIdx = parent->find(*node);
            if (auto* stringnode = parent->insertChild<treeview::VariableNode>(nIdx + 1, strNewName))
                stringnode->setValue("enter text here");
        }
    }
    ImGui::Separator();
    if (ImGui::MenuItem("Convert Option -> Data Variable", NULL, false, false))
    {
    }
    if (ImGui::MenuItem("Convert Data -> Option Variable", NULL, false, false))
    {
    }
    if (ImGui::MenuItem("Convert Folder -> Exclusive Folder", NULL, false, 
        folder && !folder->getExclusive()))
    {
        folder->setExclusive(true);
    }
    if (ImGui::MenuItem("Convert Exclusive -> Folder", NULL, false,
        folder && folder->getExclusive()))
    {
        folder->setExclusive(false);
    }
    ImGui::Separator();
    if (ImGui::MenuItem("Duplicate Layer(s)", NULL, false, true))
    {
        int nIdx = parent->find(*node);
        parent->dup(nIdx);
    }
    if (ImGui::MenuItem("Delete Layer(s)", NULL, false, true))
    {
        int nIdx = parent->find(*node);
        parent->remove(nIdx);
    }
}


void panels::VariablePanel::ClickExclusiveIcon(treeview::Node* node)
{
    bool bFindFlag = false;
    bool bSetActiveFlag = false;

    for (auto& it : *node)
    {
        treeview::VariableNode* childNode = (treeview::VariableNode*)it.get();
        bool childActive = childNode->getVis();
        if (bFindFlag)
        {
            childNode->setVis(true);
            bSetActiveFlag = true;
            break;
        }
        if (childActive)
        {
            childNode->setVis(false);
            bFindFlag = true;
        }
    }
    if (bFindFlag == false || (bFindFlag && bSetActiveFlag == false))
    {
        for (auto& it : *node)
        {
            treeview::VariableNode* childNode = (treeview::VariableNode*)it.get();
            childNode->setVis(true);
            break;
        }
    }
}

void panels::VariablePanel::ClickOptionValueIcon(treeview::Node* node)
{
    treeview::Node* parentNode = node->getParent();
    if (GET_PARENT_EXCLUSICE(node))
    {
        for (auto& it : *parentNode)
        {
            treeview::VariableNode* childNode = (treeview::VariableNode*)it.get();
            if (childNode == node)
            {
                childNode->setVis(true);
            }
            else
            {
                childNode->setVis(false);
            }
        }
    }
    else
    {
        treeview::VariableNode* childNode = (treeview::VariableNode*)node;
        bool bActive = (treeview::VariableNode*)childNode->getVis();
        childNode->setVis(!bActive);
    }
}

void panels::VariablePanel::ClickDataValueIcon(treeview::Node* node)
{
    int aaa = 0;

}