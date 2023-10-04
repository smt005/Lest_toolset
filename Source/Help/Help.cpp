
#include "Help.h"
#include "FileManager.h"

using namespace help;

int InputContainer::guiIndex = 0;
void InputContainer::Clear() {
	textBuffer[0] = '\0';
	_guiId = "##" + std::to_string(++guiIndex);
}
void InputContainer::Set(const std::string& text) {
	_guiId = "##" + std::to_string(++guiIndex);
	help::CopyToArrayChar(textBuffer, text);
}

//...
bool help::LoadJson(const std::string& fileName, Json::Value& value) {
	std::string stringData = FileManager::ReadTextFile(fileName);

	Json::CharReaderBuilder readerBuilder;
	Json::CharReader *reader = readerBuilder.newCharReader();
	std::string err;
	if (reader->parse(stringData.c_str(), stringData.c_str() + stringData.length(), &value, &err)) {
		return true;
	}

	help::Log("Fail parse: " + fileName);
	return false;
}

bool help::SaveJson(const std::string& fileName, const Json::Value& value, const std::string& indentation) {
	const std::string stringData = StringFromJson(value, indentation);
	return FileManager::WriteTextFile(fileName, stringData);
}

std::string help::StringFromJson(const Json::Value& value, const std::string& indentation) {
	Json::StreamWriterBuilder writerBuilder;
	writerBuilder["indentation"] = indentation;
	return Json::writeString(writerBuilder, value);
}
