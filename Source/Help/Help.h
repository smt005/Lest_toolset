#pragma once

#define WINDOWS_HIDE_CONSOLE true

#include <string>
#include <iostream>
#include "json/json.h"

namespace help {
	typedef std::array<char, 128> TextBuffer;

	template <class T>
	void Log(T&& text) {
#if !WINDOWS_HIDE_CONSOLE
		std::cout << "LOG: " << text << std::endl;
#endif

#ifdef _DEBUG
		_CrtDbgReport(_CRT_WARN, NULL, 0, NULL, "LOG: %s\n", text.c_str());
#endif
	}

	template <typename T>
	inline void CopyToArrayChar(T& arrayChar, const std::string& text) {
		strncpy(arrayChar.data(), text.data(), arrayChar.size());
	};

	//...
	struct InputContainer {
		typedef std::shared_ptr<InputContainer> Ptr;

		InputContainer() : _guiId("##" + std::to_string(++guiIndex)) {
		}
		const char* GetId() {
			return _guiId.c_str();
		}
		void Clear();
		void Set(const std::string& text);

		TextBuffer textBuffer;

	private:
		std::string _guiId;
		static int guiIndex;
	};

	//...
	bool LoadJson(const std::string& fileName, Json::Value& value);
	bool SaveJson(const std::string& fileName, const Json::Value& value, const std::string& indentation = "\t");
	std::string StringFromJson(const Json::Value& value, const std::string& indentation = "\t");
};
