#pragma once

#include <string>
#include <memory>
#include <map>

class Texture final {
public:
	typedef std::shared_ptr<Texture> Ptr;

public:
	
	Texture(unsigned int id, const std::string& fileName)
		: _id(id)
		, _width(0)
		, _height(0)
		, _fileName(fileName)
	{}
	~Texture();

	Texture() = delete;
	Texture(const Texture&) = delete;
	Texture(Texture&&) = delete;
	void operator=(const Texture&) = delete;
	void operator=(Texture&&) = delete;

	unsigned int operator * () const {
		return _id;
	}

	operator bool () const {
		return _id != 0;
	}

	unsigned int GetWidth() const { return _width; }
	unsigned int GetHeight() const { return _height; }

	void Delete();
	void Load();

private:
	unsigned int _id;
	unsigned int _width;
	unsigned int _height;
	std::string _fileName;

public:
	static Texture::Ptr AddTexture(const std::string& fileName, unsigned int id);
	static Texture::Ptr GetTexture(const std::string& fileName);
	static void RemoveTexture(const std::string& fileName);
	static unsigned int LoadTexture(const std::string& fileName, unsigned int& width, unsigned int& height);

private:
	static std::map<std::string, Texture::Ptr> textures;
};
