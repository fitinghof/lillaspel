#pragma once

#include <string>
#include <filesystem>

class FilepathHolder {
public:
	static void SetDirectiories() { 
		std::filesystem::path exePath = std::filesystem::canonical(L"lillaspel.exe");
		exeDirectory = exePath.parent_path();

		std::filesystem::path assetsPath = exeDirectory / ".." / ".." / ".." / ".." / "assets";
		assetsDirectory = std::filesystem::canonical(assetsPath);
	}

	static inline const std::filesystem::path& GetExeDirectory() { return exeDirectory; }
	static inline const std::filesystem::path& GetAssetsDirectory() { return assetsDirectory; }

private:
	static std::filesystem::path exeDirectory;
	static std::filesystem::path assetsDirectory;
};
