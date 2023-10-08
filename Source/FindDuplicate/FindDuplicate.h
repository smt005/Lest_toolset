#pragma once

#include <any>
#include <filesystem>
#include "../Objects/Properies.h"
#include "../Objects/Objects.h"
#include  "../Help/Help.h"


namespace Window {
	class FindDuplicate final {
	public:
		struct DisplayProperty {
			typedef std::shared_ptr<DisplayProperty> Ptr;

			template <typename T>
			DisplayProperty(const std::string& argName, T&& argPropertyPtr)
				: name(argName)
				, propertyPtr(argPropertyPtr)
			{ }

			std::string name;
			std::any displayPropertyPtr;
			property::Object::Property propertyPtr;
		};

		struct SelectObjectData {
			help::InputContainer<64> inpetIdText;
			std::string textInfo;
			property::Object::Ptr objectPtr;
			std::vector<DisplayProperty::Ptr> properties;

			void Set(property::Object::Ptr& argObjectPtr) {
				objectPtr = argObjectPtr;
				properties.clear();
				textInfo.clear();
				inpetIdText.Set(objectPtr->GetId());
			}

			void Clear() {
				inpetIdText.Clear();
				textInfo.clear();
				properties.clear();
			}
		};

	// FindDuplicate

	public:
		FindDuplicate(const std::filesystem::path& resourcePathName, const std::string& resourceFileName);
		FindDuplicate() = delete;
		FindDuplicate(const FindDuplicate&) = delete;
		FindDuplicate(FindDuplicate&&) = delete;
		void operator = (const FindDuplicate&) = delete;
		void operator = (FindDuplicate&&) = delete;

	public:
		void InitDisplay(float width, float height);
		void Render();
		void Resize(float width, float height);

		void Load();
		void Save();

	private:
		inline std::shared_ptr<bool>& GetWndPtr() { return _flagWndPtr ? _flagWndPtr : _flagWndPtr = std::make_shared<bool>(true); }
		inline void ResetPtr() { _flagWndPtr.reset(); }

		void ListDisplay();
        void DisplayId();
        void EditDisplay();
        void ButtonDisplay();

		void ShowAdd();
		void ShowRemove();
		void ShowRemoveProperty(property::Object::Ptr& objectPtr, DisplayProperty::Ptr& displayPropPtr);
		void ShowAddProperty();

		void Select(const std::string& id);
		bool CheckObjectId(const char* id);
		bool Add(const std::string& newId);
		void Remove();
        

	private:
		const char* _windowId;
		const int _windowFlags;
		std::shared_ptr<bool> _flagWndPtr;

		std::string _resourceFileName;
		SelectObjectData _selectObjectData;
		std::vector<property::Object::Ptr> _objects;
	};
}
