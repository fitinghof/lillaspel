#include "objects/objectLoader.h"


ObjectLoader::ObjectLoader() {
}

ObjectLoader::~ObjectLoader() {
}

void ObjectLoader::LoadGltf(std::string path) {
	auto gltfFile = fastgltf::GltfDataBuffer::FromPath("./assets/someasset");
	auto data = fastgltf::Parser::loadGltf(gltfFile);
}