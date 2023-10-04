#pragma once

#include <string>
#include <filesystem>

namespace help {
	class FileManager {
	public:
		static void SetResourcesDir(const std::filesystem::path& argResourcesDir);
		static const std::filesystem::path& GetResourcesDir();
		static std::filesystem::path FullPath(const std::filesystem::path& fileName);
		static bool FileManager::HasFile(const std::filesystem::path& fileName);

		static std::string ReadTextFile(const std::filesystem::path& fileName);
		static bool WriteTextFile(const std::filesystem::path& fileName, const std::string& text);

	private:
		static std::filesystem::path resourcesDir;
	};
}
