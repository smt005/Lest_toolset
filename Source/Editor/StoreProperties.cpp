
#include "StoreProperties.h"

#include <functional>
#include <charconv>

namespace Editor {
    using namespace property;

    ////////////////////////////////////////////////////////////////////////////////
    // Вспомогательные функции
    ////////////////////////////////////////////////////////////////////////////////
    
    const std::vector<const char*>& GetListProperties() {
        static const std::vector<const char*> listProperties = { "Icon", "Material", "Speed", "Points", "Int" };
        return listProperties;
    }

    namespace {
        Material::Type GetType(const std::string& materialStr) {
            if (materialStr == "Steel") {
                return Material::Type::Steel;
            }
            else if (materialStr == "Stone") {
                return Material::Type::Stone;
            }
            else if (materialStr == "Wood") {
                return Material::Type::Wood;
            }
            return Material::Type::None;
        }

        std::string GetTypeString(Material::Type type) {
            std::string materialStr;

            switch (type)
            {
            case Material::Type::Steel: materialStr = "Steel"; break;
            case Material::Type::Stone: materialStr = "Stone"; break;
            case Material::Type::Wood: materialStr = "Wood"; break;
            default:
                break;
            }

            return materialStr;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    // Сохранение
    ////////////////////////////////////////////////////////////////////////////////

    void Saver::operator()(const Icon::Ptr& icon) {
        _data["Icon"] = icon->fileName;
    }

    void Saver::operator()(const Material::Ptr& materialPtr) {
        std::string materialStr = GetTypeString(materialPtr->type);
        if (!materialStr.empty()) {
            _data["Material"] = materialStr;
        }
    }

    void Saver::operator()(const Points::Ptr& points) {
        _data["Points"] = std::to_string(points->value);
    }

    void Saver::operator()(const Speed::Ptr& speedPtr) {
        _data["Speed"] = speedPtr->value;
        _data["SpeedLimits"][0] = speedPtr->minValue;
        _data["SpeedLimits"][1] = speedPtr->maxValue;
    }

    void Saver::operator()(const IntPtr& intPtr) {
        _data["Int"] = *intPtr;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // Загрузка
    ////////////////////////////////////////////////////////////////////////////////

    template<typename T>
    struct Loader {
        static bool Load(std::vector<Object::Property>& properties, const Json::Value& data) { }
    };

    template<>
    struct Loader<Icon> {
        static bool Load(std::vector<Object::Property>& properties, const Json::Value& data) {
            if (!data["Icon"].empty()) {
                std::string fileName = data["Icon"].asString();
                auto ptr = std::make_shared<Icon>(std::move(fileName));
                properties.emplace_back(ptr);
                return true;
            }

            return false;
        }
    };

    template<>
    struct Loader<Material> {
        static bool Load(std::vector<Object::Property>& properties, const Json::Value& data) {
            if (!data["Material"].empty()) {
                std::string materialStr = data["Material"].asString();
                Material::Type type = GetType(materialStr);

                auto ptr = std::make_shared<Material>(type);
                properties.emplace_back(ptr);
                return true;
            }

            return false;
        }
    };

    template<>
    struct Loader<Points> {
        static bool Load(std::vector<Object::Property>& properties, const Json::Value& data) {
            if (!data["Points"].empty()) {
                std::string valueStr = data["Points"].asString();
                unsigned long value = 0;
                
                if (std::from_chars(valueStr.data(), valueStr.data() + valueStr.size(), value).ec != std::errc{}) {
                    help::Log("Fail format Points: " + valueStr);
                }

                auto ptr = std::make_shared<Points>(value);
                properties.emplace_back(ptr);
                return true;
            }

            return false;
        }
    };

    template<>
    struct Loader<Speed> {
        static bool Load(std::vector<Object::Property>& properties, const Json::Value& data) {
            if (!data["Speed"].empty()) {
                float value = data["Speed"].asFloat();
                float minValue = value;
                float maxValue = value;

                if (!data["SpeedLimits"].empty()) {
                    auto values = data["SpeedLimits"];
                    size_t size = values.size();

                    if (size == 2) {
                        minValue = values.begin()->asFloat();
                        maxValue = std::next(values.begin())->asFloat();
                    }
                    else if (size == 1) {
                        maxValue = values.begin()->asFloat();
                    }
                }

                minValue = minValue > value ? value : minValue;
                maxValue = maxValue < value ? value : maxValue;

                auto ptr = std::make_shared<Speed>(value, minValue, maxValue);
                properties.emplace_back(ptr);
                return true;
            }

            return false;
        }
    };

    template<>
    struct Loader<Int> {
        static bool Load(std::vector<Object::Property>& properties, const Json::Value& data) {
            if (!data["Int"].empty()) {
                unsigned int value = data["Int"].asInt();
                auto ptr = std::make_shared<Int>(value);
                properties.emplace_back(ptr);
                return true;
            }

            return false;
        }
    };

    void LoadProperty(std::vector<Object::Property>& properties, const Json::Value& data) {
        if (!Loader<Icon>::Load(properties, data)) {}
        else if (!Loader<Material>::Load(properties, data)) {}
        else if (!Loader<Points>::Load(properties, data)) {}
        else if (!Loader<Speed>::Load(properties, data)) {}
        Loader<Int>::Load(properties, data);
    }

    Object::Property* CreateProperty(std::vector<Object::Property>& properties, const std::string& name) {
        if (name == "Icon") {
            auto ptr = std::make_shared<Icon>();
            return &properties.emplace_back(ptr);
        }
        else if (name == "Material") {
            auto ptr = std::make_shared<Material>();
            return &properties.emplace_back(ptr);
        }
        else if (name == "Points") {
            auto ptr = std::make_shared<Points>();
            return &properties.emplace_back(ptr);
        }
        else if (name == "Speed") {
            auto ptr = std::make_shared<Speed>();
            return &properties.emplace_back(ptr);
        }
        else if (name == "Int") {
            auto ptr = std::make_shared<Int>(0);
            return &properties.emplace_back(ptr);
        }

        return nullptr;
    }
}
