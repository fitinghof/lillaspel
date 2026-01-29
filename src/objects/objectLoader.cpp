#include "objects/objectLoader.h"
#include "utilities/logger.h"


ObjectLoader::ObjectLoader() {
}

ObjectLoader::~ObjectLoader() {
}

void ObjectLoader::LoadGltf(std::filesystem::path path) {
	fastgltf::Parser parser;

	auto gltfFile = fastgltf::GltfDataBuffer::FromPath(path);
	if (gltfFile.error() != fastgltf::Error::None) {
		Logger::Error("Failed to load gltf file");
		Logger::Error(std::to_string((int)gltfFile.error()));
		throw std::runtime_error("Failed");
	}

	auto data = parser.loadGltf(gltfFile.get(), path.parent_path());

	if (data.error() != fastgltf::Error::None) {
		Logger::Error("Failed to load glTF file");
		Logger::Error(std::to_string((int)data.error()));
		throw std::runtime_error("Failed");
	}

	std::vector<fastgltf::math::fvec3> positions;
	auto& asset = data.get();
	for (const auto& mesh : asset.meshes) {
		for (const auto& primitive : mesh.primitives) {
			auto* positionIt = primitive.findAttribute("POSITION");
			auto& positionAccessor = asset.accessors[positionIt->accessorIndex];

			if (!positionAccessor.bufferViewIndex.has_value())
				continue;

			positions.reserve(positionAccessor.count);
			fastgltf::iterateAccessor<fastgltf::math::fvec3>(asset, positionAccessor, [&](fastgltf::math::fvec3 position) {
				positions.emplace_back(position);
			});


		}
	}
}