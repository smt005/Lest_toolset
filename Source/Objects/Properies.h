#pragma once

#include <string>
#include <memory>

namespace property {
	struct Icon {
		typedef std::shared_ptr<Icon> Ptr;

		Icon() = default;
		Icon(const std::string& argFileName) : fileName(argFileName) {}

		std::string fileName;
	};

	struct Material {
		typedef std::shared_ptr<Material> Ptr;

		enum class Type {
			None,
			Steel,
			Stone,
			Wood
		};
		typedef std::shared_ptr<Type> MaterialPtr;

		Material() = default;
		Material(Type arg) : type(arg) {}

		Type type = Type::None;
	};

	struct Speed {
		typedef std::shared_ptr<Speed> Ptr;

		Speed() = default;
		Speed(float argValue, float argMinValue, float argMaxValue)
			: value(argValue)
			, minValue(argMinValue)
			, maxValue(argMaxValue)
		{}

		float value = 1.f;
		float minValue = 0.f;
		float maxValue = 10.f;
	};

	struct Points {
		typedef std::shared_ptr<Points> Ptr;

		Points() = default;
		Points(unsigned long arg) : value(arg) {}
		unsigned long value = 0;
	};

	typedef int Int;
	typedef std::shared_ptr<Int> IntPtr;
}
