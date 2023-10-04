#pragma once

#include "json/json.h"

#include "../Objects/Properies.h"
#include "../Objects/Objects.h"
#include "../Help/Help.h"

namespace Editor {
    const std::vector<const char*>& GetListProperties();

    // Сохранение
    class Saver final {
    public:
        Saver(Json::Value& data) : _data(data) {}

        void operator()(const property::Icon::Ptr& iconPtr);
        void operator()(const property::Material::Ptr& materialPtr);
        void operator()(const property::Points::Ptr& pointsPtr);
        void operator()(const property::Speed::Ptr& speedPtr);
        void operator()(const property::IntPtr& intPtrPtr);

    private:
        Json::Value& _data;

    public:
        static void Save(const property::Object::Property& property, Json::Value& data) {
            std::visit(Saver(data), property);
        }
    };

    // Загрузка
    void LoadProperty(std::vector<property::Object::Property>& properties, const Json::Value& data);

    // Получить новый параметр по имени
    property::Object::Property* CreateProperty(std::vector<property::Object::Property>& properties, const std::string& name);

    // Текстовое представление параметра
    class PropertName final {
    public:
        std::string operator()(const property::Icon::Ptr& propertyPtr)      { return "Icon"; }
        std::string operator()(const property::Material::Ptr& propertyPtr)  { return "Material"; }
        std::string operator()(const property::Points::Ptr& propertyPtr)    { return "Points"; }
        std::string operator()(const property::Speed::Ptr& propertyPtr)     { return "Speed"; }
        std::string operator()(const property::IntPtr& propertyPtr)         { return "Int"; }

    public:
        static std::string GetName(property::Object::Property& property) {
            return std::visit(PropertName(), property);
        }
    };
}
