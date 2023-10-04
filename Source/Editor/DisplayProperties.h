#pragma once

#include "../Objects/Objects.h"
#include "../Editor/ObjectEditor.h"

namespace Window {
    // Отображение
    class DrawProperty final {
    public:
        DrawProperty(ObjectEditor::DisplayProperty& displayProp, const float widthContent)
            : _displayProp(displayProp)
            , _widthContent(widthContent)
        {}

        void operator()(property::Icon::Ptr& icon);
        void operator()(property::Material::Ptr& material);
        void operator()(property::Points::Ptr& points);
        void operator()(property::Speed::Ptr& speed);
        void operator()(property::IntPtr& intPtr);

        template<typename T>
        T* GetDisplayPropertyPtr(std::any& displayProperty) const {
            if (displayProperty.type() != typeid(std::shared_ptr<T>)) {
                return nullptr;
            }
            return std::any_cast<std::shared_ptr<T>>(_displayProp.displayPropertyPtr).get();
        }

    public:
        static void DrawTitle(const std::string& text);
        static void Draw(ObjectEditor::DisplayProperty& property, float widthContent) {
            std::visit(DrawProperty(property, widthContent), property.propertyPtr);
        }

    private:
        ObjectEditor::DisplayProperty& _displayProp;
        float _widthContent;

    public:
        static float widthContentBorder;
    };

    // Подготовка к отображению
    class PrepareProperty final {
    public:
        PrepareProperty(std::vector<Window::ObjectEditor::DisplayProperty::Ptr>& properties) : _properties(properties) { }

        template <typename T>
        std::any& AddDisplayProperty(const std::string& displayText, T&& prop) const {
            return _properties.emplace_back(new ObjectEditor::DisplayProperty(displayText, prop))->displayPropertyPtr;
        }

        void operator()(property::Icon::Ptr& property);
        void operator()(property::Material::Ptr& property);
        void operator()(property::Points::Ptr& property);
        void operator()(property::Speed::Ptr& property);
        void operator()(property::IntPtr& property);

    public:
        static void Prepare(property::Object::Property& property, std::vector<Window::ObjectEditor::DisplayProperty::Ptr>& properties) {
            std::visit(PrepareProperty(properties), property);
        }

    private:
        std::vector<Window::ObjectEditor::DisplayProperty::Ptr>& _properties;
    };
}