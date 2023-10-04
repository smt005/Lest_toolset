
#include "Texture.h"

#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_opengl3_loader.h"
#include "../FreeImage/src/FreeImage.h"

#include "../Help/FileManager.h"

std::map<std::string, Texture::Ptr> Texture::textures;

Texture::~Texture() {
	Delete();
}

void Texture::Delete() {
	glDeleteTextures(1, &_id);
	_id = 0;
}

void Texture::Load() {
	if (_id == 0 && !_fileName.empty()) {
		_id = LoadTexture(_fileName, _width, _height);
	}
}

// Static 

Texture::Ptr Texture::AddTexture(const std::string& fileName, unsigned int id) {
	auto it = textures.find(fileName);
	if (it != textures.end()) {
		return it->second;
	}

	auto newIt = textures.emplace(fileName, new Texture(id, fileName));
	return newIt.first->second;
}

Texture::Ptr Texture::GetTexture(const std::string& fileName) {
	auto it = textures.find(fileName);
	if (it != textures.end()) {
		return it->second;
	}
	return {};
}

void Texture::RemoveTexture(const std::string& fileName) {
	auto it = textures.find(fileName);
	if (it != textures.end()) {
		textures.erase(it);
	}
}

unsigned int Texture::LoadTexture(const std::string& fileName, unsigned int& width, unsigned int& height) {
	width = 0;
	height = 0;
	std::filesystem::path fullFileName = help::FileManager::FullPath(fileName);

	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP* bitMap = nullptr;
	BYTE* bytes = nullptr;

	fif = FreeImage_GetFileType(fullFileName.string().c_str(), 0);

	if (fif == FIF_UNKNOWN) {
		fif = FreeImage_GetFIFFromFilename(fullFileName.string().c_str());
	}

	if (fif == FIF_UNKNOWN) {
		return 0;
	}

	if (FreeImage_FIFSupportsReading(fif)) {
		bitMap = FreeImage_Load(fif, fullFileName.string().c_str());
	}

	if (!bitMap) {
		return 0;
	}

	width = FreeImage_GetWidth(bitMap);
	height = FreeImage_GetHeight(bitMap);
	bytes = FreeImage_GetBits(bitMap);

	if ((bytes == 0) || (width == 0) || (height == 0)) {
		width = 0;
		height = 0;
		FreeImage_Unload(bitMap);
		return 0;
	}

	unsigned int textureId = 0;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	if (FreeImage_GetBPP(bitMap) == 32) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bytes);
	}
	else if (FreeImage_GetBPP(bitMap) == 24) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, bytes);
	}
	else {
		FreeImage_Unload(bitMap);
		width = 0;
		height = 0;
		return 0;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	FreeImage_Unload(bitMap);
	return textureId;
}
