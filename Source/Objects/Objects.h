#pragma once

#include <variant>
#include <vector>

#include "Properies.h"

namespace property {
	class Object {
	public:
		typedef std::variant<Icon::Ptr, Material::Ptr, Speed::Ptr, Points::Ptr, IntPtr> Property;
		typedef std::shared_ptr<Object> Ptr;

	public:
		Object() = delete;
		Object(const std::string& id) : _id(id) { }
		Object(std::string&& id) : _id(id) { }

		~Object() = default;

		void operator = (const std::string& id) = delete;
		void operator = (std::string&& id) = delete;

		const std::string& GetId() const {
			return _id;
		}

		void SetId(const std::string& id) {
			_id = id;
		}

		std::vector<Property>& GetProperties() {
			return _properties;
		}

	private:
		std::string _id;
		std::vector<Property> _properties;
	
	};
}
