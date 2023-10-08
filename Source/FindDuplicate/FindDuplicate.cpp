#include "FindDuplicate.h"

#include <set>
#include "imgui.h"

#include "../FindDuplicate/DisplayProperties.h"
#include "../FindDuplicate/StoreProperties.h"
#include "../Editor/PopupModal.h"
#include "../Objects/Objects.h"
#include "../Help/FileManager.h"
#include "../Help/WindowsWin.h"

namespace Window {
    using namespace property;

    namespace {
        ImVec2 buttonSize = { 90.f, 32.f };
        ImVec2 wideButtonSize = { 120.f, 32.f };
        ImVec2 listSize = { 200.f, 535.f };
        ImVec2 propertySize = { 540.f, 535.f };
        ImVec2 propButtonsSize = { 540.f, 40.f };
        float border = 25.f;
        float offsetRightButtons = 550.f;
        float borderRightButtons = 275.f;

        ImVec4 redColor = {0.9f, 0.1f, 0.1f, 1.f};
        ImVec4 blackColor = { 0.0f, 0.0f, 0.0f, 1.f };
        ImVec4 whiteColor = { 1.f, 1.f, 1.f, 1.f };
        ImVec4 grayColor = { 0.9f, 0.9f, 0.9f, 1.f };
        ImVec4 greenColor = { 0.9f, 0.9f, 0.9f, 1.f };
    }

    FindDuplicate::FindDuplicate(const std::filesystem::path& resourcePath, const std::string& resourceFileName)
        : _windowId(typeid(FindDuplicate).name())
        , _windowFlags(ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove) 
        , _resourceFileName(resourceFileName)
	{
        std::filesystem::path path = std::filesystem::current_path();
        path += resourcePath;
        help::FileManager::SetResourcesDir(path);
	}

	void FindDuplicate::Resize(float width, float height) {        
        ImGui::SetWindowPos(_windowId, { 0.f, 0.f });
		ImGui::SetWindowSize(_windowId, { width, height });

        propertySize.x = (width - listSize.x) - border;
        propertySize.y = height - propButtonsSize.y - border;
        listSize.y = propertySize.y;
        propButtonsSize.x = width - border;
        offsetRightButtons = width - borderRightButtons;
	}

    void FindDuplicate::InitDisplay(float width, float height) {
        ImGui::Begin(_windowId, nullptr, _windowFlags);
        Resize(width, height);
        ImGui::End();
    }

	void FindDuplicate::Render() {
		ImGui::Begin(_windowId, nullptr, _windowFlags);

        ListDisplay();

        ImGui::SameLine();
        EditDisplay();

        ImGui::Dummy(ImVec2(0.f, 0.f));
        ButtonDisplay();

        PopupModal::Draw();

		ImGui::End();
	}

    void FindDuplicate::Load() {
        Json::Value data;
        if (!help::LoadJson(_resourceFileName, data)) {
            return;
        }

        for (auto it = data.begin(); it != data.end(); ++it) {
            Object* objectTemp = new Object(it.name());
            if (!objectTemp) {
                continue;
            }

            Object::Ptr& newObjectPtr = _objects.emplace_back(objectTemp);

            if (!it->isObject()) {
                continue;
            }

            Editor::LoadProperty(newObjectPtr->GetProperties(), *it);
        }
    }

    void FindDuplicate::Save() {
        Json::Value data;

        for (const Object::Ptr& object : _objects) {
            Json::Value& objectData = data[object->GetId()];

            for (const Object::Property& property : object->GetProperties()) {
                Editor::Saver::Save(property, objectData);
            }
        }

        help::SaveJson(_resourceFileName, data);
    }

    // Private

    void FindDuplicate::ListDisplay() {
        ImGui::BeginChild("list", listSize, true);

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, listSize.x);
        ImGui::SetColumnWidth(1, 1.f);

        for (Object::Ptr& objectPtr : _objects) {
            bool select = _selectObjectData.objectPtr && (_selectObjectData.objectPtr == objectPtr);

            ImGui::TextColored(select ? redColor : blackColor, "%s", objectPtr->GetId().c_str());
            if (ImGui::IsItemHovered()) {
                if (ImGui::IsItemClicked()) {
                    Select(objectPtr->GetId());
                }
            }
            ImGui::NextColumn();
            ImGui::NextColumn();
        }

        ImGui::EndChild();
    }

    void FindDuplicate::DisplayId() {
        if (!_selectObjectData.objectPtr) {
            return;
        }

        DrawProperty::DrawTitle("Id");

        ImGui::PushItemWidth(propertySize.x / 2 - DrawProperty::widthContentBorder);
        ImGui::PushStyleColor(ImGuiCol_WindowBg | ImGuiCol_Border, _selectObjectData.textInfo.empty() ? whiteColor : redColor);

        auto& inputIdText = _selectObjectData.inpetIdText.textBuffer;

        if (ImGui::InputText(_selectObjectData.inpetIdText.GetId(), inputIdText.data(), inputIdText.size())) {
            if (_selectObjectData.objectPtr->GetId() == inputIdText.data() || CheckObjectId(inputIdText.data())) {
                _selectObjectData.objectPtr->SetId(inputIdText.data());
                _selectObjectData.textInfo.clear();
            }
            else {
                _selectObjectData.textInfo = std::string(inputIdText.data()) + " has already!";
            }
        }

        ImGui::Dummy(ImVec2(0.f, 0.f));
        ImGui::TextColored(redColor, "%s", _selectObjectData.textInfo.c_str());

        ImGui::PopStyleColor();
        ImGui::PopItemWidth();

        ImGui::Separator();
    }

    void FindDuplicate::EditDisplay() {
        int _guiId = 0;
        ImGui::BeginChild("edit", propertySize, true);

        if (_selectObjectData.objectPtr) {
            DisplayId();

            for (DisplayProperty::Ptr& displayProp : _selectObjectData.properties) {
                DisplayProperty& property = *displayProp;

                ImGui::Dummy(ImVec2(0.f, 1.f));
                DrawProperty::Draw(property, propertySize.x);

                ImGui::Dummy(ImVec2(0.f, 4.f));
                ImGui::PushID(++_guiId);
                if (ImGui::Button("Remove property", wideButtonSize)) {
                    ShowRemoveProperty(_selectObjectData.objectPtr, displayProp);
                }
                ImGui::PopID();

                ImGui::Separator();
            }
        }

        ImGui::EndChild();
    }

    void FindDuplicate::ButtonDisplay() {
        ImGui::BeginChild("button", propButtonsSize, false);

        if (ImGui::Button("Add", buttonSize)) {
            ShowAdd();
        }

        ImGui::SameLine();

        if (_selectObjectData.objectPtr) {
            ImGui::SameLine();
            if (ImGui::Button("Remove", buttonSize)) {
                ShowRemove();
            }

            ImGui::SameLine(220.f);
            if (ImGui::Button("Add property", wideButtonSize)) {
                ShowAddProperty();
            }
        }

        ImGui::SameLine(offsetRightButtons);
        if (ImGui::Button("Save", wideButtonSize)) {
            Save();
        }

        ImGui::SameLine();
        if (ImGui::Button("Close", wideButtonSize)) {
            exit(1);
        }

        ImGui::EndChild();
    }

    void FindDuplicate::ShowAdd() {
        PopupModal::Show(GetWndPtr(), [this, inputPtr = std::make_shared<std::array<char, 64>>(), infoTextPtr = std::make_shared<std::string>()]() {
            ImGui::PushItemWidth(190.f);
            ImGui::PushStyleColor(ImGuiCol_WindowBg | ImGuiCol_Border, infoTextPtr->empty() ? grayColor : redColor);

            if (ImGui::InputText("##new_id_input", inputPtr->data(), inputPtr->size())) {
                if ((*inputPtr)[0] == '\0') {
                    *infoTextPtr = "Enter ID";
                }
                else {
                    if (CheckObjectId(inputPtr->data())) {
                        infoTextPtr->clear();
                    }
                    else {
                        *infoTextPtr = std::string(inputPtr->data()) + " has already!";
                    }
                }
            }
            
            ImGui::PopStyleColor();
            ImGui::PopItemWidth();

            ImGui::TextColored(redColor, "%s", infoTextPtr->c_str());

            ImGui::Dummy(ImVec2(0.f, 0.f));
            if (ImGui::Button("Add", buttonSize) && (infoTextPtr->empty() && (*inputPtr)[0] != '\0')) {
                if (Add(inputPtr->data())) {
                    PopupModal::Hide();
                }
            }

            ImGui::SameLine();
            if (ImGui::Button("Close", buttonSize)) {
                PopupModal::Hide();
            }
        }, "Add");
    }

    void FindDuplicate::ShowRemove() {
        PopupModal::Show(GetWndPtr(), [this]() {
            ImGui::TextColored(redColor, "Remove %s?", _selectObjectData.objectPtr->GetId().c_str());

            ImGui::Dummy(ImVec2(0.f, 0.f));
            if (ImGui::Button("Remove", buttonSize)) {
                Remove();
                PopupModal::Hide();
            }

            ImGui::SameLine();
            ImGui::SameLine();
            if (ImGui::Button("Close", buttonSize)) {
                PopupModal::Hide();
            }
        }, "Remove");
    }

    void FindDuplicate::ShowAddProperty() {
        std::set<const char*> actualSetProperties(Editor::GetListProperties().begin(), Editor::GetListProperties().end());

        for (auto& prop : _selectObjectData.objectPtr->GetProperties()) {
            auto it = std::find_if(actualSetProperties.begin(), actualSetProperties.end(), [name = Editor::PropertName::GetName(prop)](const char* charPtr) {
                return name == charPtr;
                });
            if (it != actualSetProperties.end()) {
                actualSetProperties.erase(it);
            }
        }
        std::shared_ptr<std::vector<const char*>> actualPropertiesPtr(new std::vector<const char*>(actualSetProperties.begin(), actualSetProperties.end()));

        PopupModal::Show(GetWndPtr(), [this, actualPropertiesPtr, propertyindex = int(0)]() {
            if (actualPropertiesPtr->empty()) {
                ImGui::TextColored({ 0.9f, 0.0f, 0.0f, 1.0f }, "%s", "Nothing to add.");
                ImGui::Dummy(ImVec2(0.f, 16.f));
            }
            else {
                ImGui::Text("Select property.");

                ImGui::PushItemWidth(190.f);
                ImGui::PushStyleColor(ImGuiCol_WindowBg | ImGuiCol_Border, { 0.9f, 0.9f, 0.9f, 1.0f });
                ImGui::Combo("##ListProperty", (int*)&propertyindex, actualPropertiesPtr->data(), actualPropertiesPtr->size());
                ImGui::PopStyleColor();
                ImGui::PopItemWidth();

                ImGui::Dummy(ImVec2(0.f, 0.f));
                if (ImGui::Button("Add", buttonSize)) {
                    if (Object::Property* newproperty = Editor::CreateProperty(_selectObjectData.objectPtr->GetProperties(), (*actualPropertiesPtr)[propertyindex])) {
                        PrepareProperty::Prepare(*newproperty, _selectObjectData.properties);
                    }

                    PopupModal::Hide();
                }
            }

            ImGui::SameLine();
            if (ImGui::Button("Close", buttonSize)) {
                PopupModal::Hide();
            }
        }, "Add property");
    }

    void FindDuplicate::ShowRemoveProperty(Object::Ptr& objectPtr, DisplayProperty::Ptr& displayPropPtr) {
        auto removeProperty = [this, objectPtr, displayPropPtr]() {
            std::vector<Object::Property>& properties = objectPtr->GetProperties();
            auto itProp = std::find_if(properties.begin(), properties.end(), [&propertyPtr = displayPropPtr->propertyPtr](const Object::Property& itProperty) {
                return itProperty == propertyPtr;
            });

            std::vector<DisplayProperty::Ptr>& displayProperties = _selectObjectData.properties;
            auto itDispProp = std::find_if(displayProperties.begin(), displayProperties.end(), [&displayPropPtr](const DisplayProperty::Ptr& itPropertyPtr) {
                return itPropertyPtr == displayPropPtr;
            });

            if (itProp != properties.end() && itDispProp != displayProperties.end()) {
                properties.erase(itProp);
                displayProperties.erase(itDispProp);
            }
        };

        PopupModal::Show(GetWndPtr(), [removeProperty]() {
            ImGui::TextColored({ 0.9f, 0.0f, 0.0f, 1.0f }, "%s", "Remove?");

            ImGui::Dummy(ImVec2(0.f, 0.f));
            if (ImGui::Button("Remove", buttonSize)) {
                removeProperty();
                PopupModal::Hide();
            }

            ImGui::SameLine();
            if (ImGui::Button("Close", buttonSize)) {
                PopupModal::Hide();
            }
        }, "Remove property");
    }

    void FindDuplicate::Select(const std::string& id) {
        auto it = std::find_if(_objects.begin(), _objects.end(), [&id](const Object::Ptr& objectPtr) {
            return objectPtr->GetId() == id;
        });

        if (it != _objects.end()) {
            _selectObjectData.Set(*it);

            std::vector<DisplayProperty::Ptr>& displayProperties = _selectObjectData.properties;
            std::vector<Object::Property>& properties = _selectObjectData.objectPtr->GetProperties();

            for (auto& prop : _selectObjectData.objectPtr->GetProperties()) {
                PrepareProperty::Prepare(prop, displayProperties);
            }
        }
    }

    bool FindDuplicate::CheckObjectId(const char* id) {
        auto it = std::find_if(_objects.begin(), _objects.end(), [id](const Object::Ptr& object) {
            return object->GetId() == id;
        });
        return it == _objects.end();
    }

    bool FindDuplicate::Add(const std::string& newId) {
        if (std::find_if(_objects.begin(), _objects.end(), [&newId](const Object::Ptr& argObject) { return argObject->GetId() == newId; }) != _objects.end()) {
            return false;
        }

        _objects.emplace_back(new Object(newId));
        Select(newId);
        return true;
    }

    void FindDuplicate::Remove() {
        if (!_selectObjectData.objectPtr) {
            return;
        }

        auto it = std::find_if(_objects.begin(), _objects.end(), [&objectPtr = _selectObjectData.objectPtr](const Object::Ptr& itObjectPtr) { return itObjectPtr == objectPtr; });
        if (it != _objects.end()) {
            _objects.erase(it);
        }

        if (_objects.empty()) {
            _selectObjectData.Clear();
        }
        else {
            Select((*_objects.begin())->GetId());
        }
    }
}
