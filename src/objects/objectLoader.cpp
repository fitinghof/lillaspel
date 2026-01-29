#include "objects/objectLoader.h"
#include "utilities/logger.h"


ObjectLoader::ObjectLoader() {
}

ObjectLoader::~ObjectLoader() {
}

Mesh ObjectLoader::LoadGltf(std::filesystem::path path) {
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

	std::vector<Vertex> verticies;
	std::vector<uint32_t> indicies;
	std::vector<SubMesh> subMeshes;
	auto& asset = data.get();
	for (const auto& mesh : asset.meshes) {
		for (const auto& primitive : mesh.primitives) {

			auto* positionIt = primitive.findAttribute("POSITION");
			auto& positionAccessor = asset.accessors[positionIt->accessorIndex];

			if (!positionAccessor.bufferViewIndex.has_value())
				continue;

			verticies.reserve(positionAccessor.count);
			indicies.reserve(positionAccessor.count);
			fastgltf::iterateAccessor<fastgltf::math::fvec3>(asset, positionAccessor, [&](fastgltf::math::fvec3 p) {
				verticies.emplace_back(Vertex{ {p.data()[0],p.data()[1],p.data()[2]},{},{}});
			});

			auto* normalIt = primitive.findAttribute("NORMAL");
			auto& normalAccessor = asset.accessors[normalIt->accessorIndex];

			if (!positionAccessor.bufferViewIndex.has_value())
				continue;

			size_t idx = 0;
			fastgltf::iterateAccessor<fastgltf::math::fvec3>(asset, normalAccessor, [&](fastgltf::math::fvec3 n) {
				verticies[idx].normal[0] = n.data()[0];
				verticies[idx].normal[1] = n.data()[1];
				verticies[idx++].normal[2] = n.data()[2];
				indicies.emplace_back(idx);
			});


			auto* uvIt = primitive.findAttribute("TEXCOORD_n");
			auto& uvAccessor = asset.accessors[uvIt->accessorIndex];

			if (!positionAccessor.bufferViewIndex.has_value())
				continue;

			idx = 0;
			fastgltf::iterateAccessor<fastgltf::math::fvec2>(asset, uvAccessor, [&](fastgltf::math::fvec2 t) {
				verticies[idx].uv[0] = t.data()[0];
				verticies[idx++].uv[1] = t.data()[1];
			});
			SubMesh subMesh(0, idx);
			subMeshes.emplace_back(subMesh);

		}
	}
	VertexBuffer vertexBuffer;
	vertexBuffer.Init(nullptr, sizeof(Vertex), verticies.size(), verticies.data());

	IndexBuffer indexBuffer;
	indexBuffer.Init(nullptr, indicies.size(), indicies.data());

	Mesh mesh(std::move(vertexBuffer), std::move(indexBuffer), std::move(subMeshes));

	return mesh;

}