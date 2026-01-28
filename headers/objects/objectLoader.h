#pragma once
#include <fastgltf/core.hpp>
#include <string>
#include <filesystem>


class ObjectLoader
{
public:
	ObjectLoader();
	~ObjectLoader();
	void LoadGltf(std::filesystem::path path);



private:

};