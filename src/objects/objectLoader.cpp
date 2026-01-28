#include "objects/objectLoader.h"
#include "utilities/logger.h"


ObjectLoader::ObjectLoader() {
}

ObjectLoader::~ObjectLoader() {
}

void ObjectLoader::LoadGltf(std::filesystem::path path) {
	fastgltf::Parser parser;

	auto gltfFile = fastgltf::GltfDataBuffer::FromPath(path);
	auto data = parser.loadGltf(gltfFile.get(), path.parent_path());

	if (!data) {
		throw std::runtime_error("Failed");
	}

	for (const auto& mesh : data.get().meshes) {
		for (const auto& primitive : mesh.primitives) {
			Logger::Log(primitive.attributes[0].name.c_str());
		}
	}
}