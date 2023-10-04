#include "DisplayProperties.h"

#include <charconv>
#include "imgui.h"

#include "../Help/Help.h"
#include "../Help/WindowsWin.h"
#include "../Objects/Texture.h"
#include "../Help/FileManager.h"

namespace Window {
    using namespace property;

    // Вспомогательные классы
    struct IconContainer {
        typedef std::shared_ptr<IconContainer> Ptr;

        help::InputContainer filePath;
        Texture::Ptr texture;

        float sizeDisplayIcon = 100.f;
    };

    struct SpeedContainer {
        typedef std::shared_ptr<SpeedContainer> Ptr;

        bool isEdit = 0;
        help::TextBuffer minText;
        help::TextBuffer maxText;
    };

    // Вспомогательная функция
    std::string ChooseFile() {
        std::string fiilNameFile = help::SelectFile();
        std::replace(fiilNameFile.begin(), fiilNameFile.end(), '\\', '/');

        std::string resourcesDir = help::FileManager::GetResourcesDir().u8string();
        std::replace(resourcesDir.begin(), resourcesDir.end(), '\\', '/');

        size_t pos = fiilNameFile.find(resourcesDir);
        if (pos == resourcesDir.npos) {
            return std::string();
        }

        pos += resourcesDir.size() + 1;
        if (pos >= fiilNameFile.size()) {
            return std::string();
        }

        size_t size = fiilNameFile.size() - pos;
        std::string nameFile = fiilNameFile.substr(pos, size);
        return nameFile;
    }

    template <typename T>
    bool UpdatePropertyTexture(T&& newFileName, IconContainer& iconContainer, property::Icon& icon) {
        if (help::FileManager::HasFile(newFileName)) {
            iconContainer.texture = Texture::AddTexture(newFileName, 0);
            icon.fileName = newFileName;
            return true;
        }

        return false;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // Отображение
    ////////////////////////////////////////////////////////////////////////////////

    float DrawProperty::widthContentBorder = 30.f;

    void DrawProperty::DrawTitle(const std::string& text) {
        ImGui::SameLine(10.f);
        ImGui::Text(text.c_str());
        ImGui::Dummy(ImVec2(0.f, 0.f));
    }

    void DrawProperty::operator()(property::Icon::Ptr& iconPtr) {
        IconContainer* iconContainerPtr = GetDisplayPropertyPtr<IconContainer>(_displayProp.displayPropertyPtr);
        if (!iconContainerPtr)
            return;

        DrawTitle(_displayProp.name);

        float widthButton = 100.f;
        float widthSlider = _widthContent - widthButton - widthContentBorder;

        ImGui::PushItemWidth(widthSlider);
        help::TextBuffer& textBuffer = iconContainerPtr->filePath.textBuffer;

        if (ImGui::InputText(iconContainerPtr->filePath.GetId(), textBuffer.data(), textBuffer.size())) {
            UpdatePropertyTexture(textBuffer.data(), *iconContainerPtr, *iconPtr);
        }
        ImGui::PopItemWidth();

        ImGui::SameLine();
        if (ImGui::Button("...##choose_texture_btn", { widthButton, 20.f })) {
            std::string newFileName = ChooseFile();
            if (UpdatePropertyTexture(newFileName, *iconContainerPtr, *iconPtr)) {
                help::CopyToArrayChar(textBuffer, newFileName);
            }
        }

        if (*iconContainerPtr->texture) {
            float width = static_cast<float>(iconContainerPtr->texture->GetWidth()) / static_cast<float>(iconContainerPtr->texture->GetHeight()) * iconContainerPtr->sizeDisplayIcon;

            ImGui::Dummy(ImVec2(0.f, 0.f));
            ImGui::Image(reinterpret_cast<ImTextureID>(**iconContainerPtr->texture),
                        { width, iconContainerPtr->sizeDisplayIcon },
                        { 0, 1 }, { 1, 0 }, { 1.f, 1.f, 1.f, 1.f }, { 1.f, 1.f, 1.f, 1.f });
        }
        else {  
            iconContainerPtr->texture->Load();
        }
    }

    void DrawProperty::operator()(property::Material::Ptr& materialPtr) {
        DrawTitle(_displayProp.name);

        int materialIndex = static_cast<int>(materialPtr->type);

        static const char* listMaterial[] = { "None", "Steel", "Stone", "Wood" };
        static int listMaterialSize = (int)(sizeof(listMaterial) / sizeof(*listMaterial));

        ImGui::PushItemWidth(_widthContent - widthContentBorder);
        if (ImGui::Combo("##material", (int*)&materialIndex, listMaterial, listMaterialSize)) {
            materialPtr->type = static_cast<property::Material::Type>(materialIndex);
        }
        ImGui::PopItemWidth();
    }

    void DrawProperty::operator()(property::Points::Ptr& pointsPtr) {
        help::InputContainer* textPtr = GetDisplayPropertyPtr<help::InputContainer>(_displayProp.displayPropertyPtr);
        if (!textPtr)
            return;

        DrawTitle(_displayProp.name);

        ImGui::PushItemWidth(_widthContent);
        help::TextBuffer& textBuffer = textPtr->textBuffer;

        if (ImGui::InputText(textPtr->GetId(), textBuffer.data(), textBuffer.size())) {
            unsigned long value;

            if (std::from_chars(textBuffer.data(), textBuffer.data() + textBuffer.size(), value).ec == std::errc{}) {
                pointsPtr->value = value;
            }

            help::CopyToArrayChar(textBuffer, std::to_string(pointsPtr->value));
        }
        ImGui::PopItemWidth();
    }

    void DrawProperty::operator()(property::Speed::Ptr& speedPtr) {
        SpeedContainer* speedContainerPtr = GetDisplayPropertyPtr<SpeedContainer>(_displayProp.displayPropertyPtr);
        if (!speedContainerPtr)
            return;

        DrawTitle(_displayProp.name);

        float widthButton = 100.f;

        if (!speedContainerPtr->isEdit) {
            float widthSlider = _widthContent - widthButton - 20.f; // 20 - для промежутка между SliderFloat и кнопкой
            ImGui::PushItemWidth(widthSlider);
            ImGui::SliderFloat("##speed", &speedPtr->value, speedPtr->minValue, speedPtr->maxValue, "%.2f");
            ImGui::PopItemWidth();

            ImGui::SameLine();
            if (ImGui::Button(">...<##limit_btn", ImVec2(widthButton, 20.f))) {
                speedContainerPtr->isEdit = true;

                help::CopyToArrayChar(speedContainerPtr->minText, std::to_string(speedPtr->minValue));
                help::CopyToArrayChar(speedContainerPtr->maxText, std::to_string(speedPtr->maxValue));
            }
        }
        else {
            float widthinputText = _widthContent - widthButton;
            widthinputText /= 2.f;
            widthinputText -= 60.f;

            ImGui::Text("Min:");

            ImGui::SameLine();
            ImGui::PushItemWidth(widthinputText);

            help::TextBuffer& minText = speedContainerPtr->minText;

            if (ImGui::InputText("##min_limit", minText.data(), minText.size())) { 
                float valueTmp = 0;
                if (std::from_chars(minText.data(), minText.data() + minText.size(), valueTmp).ec == std::errc{}) {
                    if (valueTmp > speedPtr->maxValue) {
                        speedPtr->minValue = speedPtr->maxValue;
                    }
                    else {
                        speedPtr->minValue = valueTmp;
                    }

                    help::CopyToArrayChar(speedContainerPtr->minText, std::to_string(speedPtr->minValue));
                }
            }

            ImGui::SameLine();
            ImGui::Text("Max:");

            ImGui::SameLine();

            help::TextBuffer& maxText = speedContainerPtr->maxText;

            if (ImGui::InputText("##max_limit", speedContainerPtr->maxText.data(), speedContainerPtr->maxText.size())) {
                float valueTmp = 0;
                if (std::from_chars(maxText.data(), maxText.data() + maxText.size(), valueTmp).ec == std::errc{}) {
                    if (valueTmp < speedPtr->minValue) {
                        speedPtr->maxValue = speedPtr->minValue;
                    }
                    else {
                        speedPtr->maxValue =  valueTmp;
                    }

                    help::CopyToArrayChar(speedContainerPtr->maxText, std::to_string(speedPtr->maxValue));
                }
            }
            ImGui::PopItemWidth();

            ImGui::SameLine();
            if (ImGui::Button("Close##close_limit_btn", ImVec2(widthButton, 20.f))) {
                speedContainerPtr->isEdit = false;

                speedPtr->value = speedPtr->value < speedPtr->minValue ? speedPtr->minValue : speedPtr->value;
                speedPtr->value = speedPtr->value > speedPtr->maxValue ? speedPtr->maxValue : speedPtr->value;
            }
        }
    }

    void DrawProperty::operator()(property::IntPtr& intPtr) {
        help::InputContainer* textPtr = GetDisplayPropertyPtr<help::InputContainer>(_displayProp.displayPropertyPtr);
        if (!textPtr)
            return;

        DrawTitle(_displayProp.name);

        ImGui::PushItemWidth(_widthContent);
        help::TextBuffer& textBuffer = textPtr->textBuffer;

        if (ImGui::InputText(textPtr->GetId(), textBuffer.data(), textBuffer.size())) {
            int value;

            if (std::from_chars(textBuffer.data(), textBuffer.data() + textBuffer.size(), value).ec == std::errc{}) {
                *intPtr = value;
            }

            help::CopyToArrayChar(textBuffer, std::to_string(*intPtr));
        }
        ImGui::PopItemWidth();
    }

    ////////////////////////////////////////////////////////////////////////////////
    // Подготовка к отображению
    ////////////////////////////////////////////////////////////////////////////////

    void PrepareProperty::operator()(property::Icon::Ptr& icon) {
        std::any& displayProperty = AddDisplayProperty("Icon", icon);
        IconContainer::Ptr iconContainer = std::make_shared<IconContainer>();
        displayProperty = iconContainer;

        const std::string& fileName = icon->fileName;
        help::CopyToArrayChar(iconContainer->filePath.textBuffer, fileName);

        iconContainer->texture = Texture::AddTexture(fileName, 0);
    }

    void PrepareProperty::operator()(property::Material::Ptr& material) {
        std::any& displayProperty = AddDisplayProperty("Material", material);
        std::shared_ptr<int> materialIndex = std::make_shared<int>(static_cast<int>(material->type));
        displayProperty = materialIndex;
    }

    void PrepareProperty::operator()(property::Points::Ptr& points) {
        std::any& displayProperty = AddDisplayProperty("Points", points);
        help::InputContainer::Ptr textBuffer = std::make_shared<help::InputContainer>();
        displayProperty = textBuffer;

        std::string valueString = std::to_string(points->value);
        help::CopyToArrayChar(textBuffer->textBuffer, valueString);
    }

    void PrepareProperty::operator()(property::Speed::Ptr& speed) {
        std::any& displayProperty = AddDisplayProperty("Speed", speed);
        SpeedContainer::Ptr speedContainer = std::make_shared<SpeedContainer>();
        displayProperty = speedContainer;

        std::string minValueString = std::to_string(speed->minValue);
        help::CopyToArrayChar(speedContainer->minText, minValueString);

        std::string maxValueString = std::to_string(speed->maxValue);
        help::CopyToArrayChar(speedContainer->maxText, maxValueString);
    }

    void PrepareProperty::operator()(property::IntPtr& intPtr) {
        std::any& displayProperty = AddDisplayProperty("int", intPtr);

        help::InputContainer::Ptr textBuffer = std::make_shared<help::InputContainer>();
        displayProperty = textBuffer;

        std::string valueString = std::to_string(*intPtr);
        help::CopyToArrayChar(textBuffer->textBuffer, valueString);
    }
}
