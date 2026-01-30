#include "gameObjects/objectLoader.h"
#include "utilities/logger.h"
#include <fastgltf/core.hpp>
#include <fastgltf/tools.hpp>
#include <fastgltf/util.hpp>


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

	constexpr auto gltfOptions =
		fastgltf::Options::DontRequireValidAssetMember |
		fastgltf::Options::AllowDouble |
		fastgltf::Options::LoadExternalBuffers |
		fastgltf::Options::LoadExternalImages |
		fastgltf::Options::GenerateMeshIndices;

	auto data = parser.loadGltf(gltfFile.get(), path.parent_path(), gltfOptions);

	if (data.error() != fastgltf::Error::None) {
		Logger::Error("Failed to load glTF file");
		Logger::Error(std::to_string((int)data.error()));
		throw std::runtime_error("Failed");
	}
	auto& asset = data.get();
	for (auto& mesh : asset.meshes) {

		std::vector<SubMesh> subMeshes;
		for (auto it = mesh.primitives.begin(); it != mesh.primitives.end(); ++it) {
			auto* positionIt = it->findAttribute("POSITION");
			assert(positionIt != it->attributes.end());
			assert(it->indicesAccessor.has_value());

			// Potentiall some dxd11 buffer creation

			if (it->materialIndex.has_value()) {
				// Apply material
			}

			auto& positionAccessor = asset.accessors[positionIt->accessorIndex];
			if (!positionAccessor.bufferViewIndex.has_value())
				continue;

			std::vector<Vertex> verticies(positionAccessor.count);
			fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec3>(asset, positionAccessor, [&](fastgltf::math::fvec3 pos, std::size_t idx) {
				verticies[idx].pos[0] = pos.x();
				verticies[idx].pos[1] = pos.y();
				verticies[idx].pos[2] = pos.z();

				verticies[idx].uv[0] = 0;
				verticies[idx].uv[1] = 0;
				});

			// Make texcord implementation

			auto& indexAccessor = asset.accessors[it->indicesAccessor.value()];
			if (!indexAccessor.bufferViewIndex.has_value())
				return false;
			//draw.count = static_cast<std::uint32_t>(indexAccessor.count);

			// Create the index buffer and copy the indices into it.
			std::vector<uint32_t> indices(indexAccessor.count);

			if (indexAccessor.componentType == fastgltf::ComponentType::UnsignedByte || indexAccessor.componentType == fastgltf::ComponentType::UnsignedShort) {
				fastgltf::copyFromAccessor<std::uint16_t>(asset, indexAccessor, indices.data());

			}
			else {
				fastgltf::copyFromAccessor<std::uint32_t>(asset, indexAccessor, indices.data());
			}

			VertexBuffer vertexBuffer;
			vertexBuffer.Init(nullptr, sizeof(Vertex), static_cast<UINT>(verticies.size()), verticies.data());

			IndexBuffer indexBuffer;
			indexbuffer.Init(nullptr, indices.size(), indices.data());

			subMeshes.emplace_back(
				vertexBuffer,
				indexBuffer   
			);

		}

	}

	return Mesh();
	
	//std::vector<Vertex> verticies;
	//std::vector<uint32_t> indicies;
	//std::vector<SubMesh> subMeshes;
	//for (const auto& mesh : asset.meshes) {
	//	for (const auto& primitive : mesh.primitives) {

	//		auto* positionIt = primitive.findAttribute("POSITION");
	//		auto& positionAccessor = asset.accessors[positionIt->accessorIndex];

	//		if (!positionAccessor.bufferViewIndex.has_value())
	//			continue;

	//		verticies.reserve(positionAccessor.count);
	//		indicies.reserve(positionAccessor.count);
	//		fastgltf::iterateAccessor<fastgltf::math::fvec3>(asset, positionAccessor, [&](fastgltf::math::fvec3 p) {
	//			verticies.emplace_back(Vertex{ {p.data()[0],p.data()[1],p.data()[2]},{},{}});
	//		});

	//		auto* normalIt = primitive.findAttribute("NORMAL");
	//		auto& normalAccessor = asset.accessors[normalIt->accessorIndex];

	//		if (!positionAccessor.bufferViewIndex.has_value())
	//			continue;

	//		size_t idx = 0;
	//		fastgltf::iterateAccessor<fastgltf::math::fvec3>(asset, normalAccessor, [&](fastgltf::math::fvec3 n) {
	//			verticies[idx].normal[0] = n.data()[0];
	//			verticies[idx].normal[1] = n.data()[1];
	//			verticies[idx++].normal[2] = n.data()[2];
	//			indicies.emplace_back(idx);
	//		});


	//		auto* uvIt = primitive.findAttribute("TEXCOORD_n");
	//		auto& uvAccessor = asset.accessors[uvIt->accessorIndex];

	//		if (!positionAccessor.bufferViewIndex.has_value())
	//			continue;

	//		idx = 0;
	//		fastgltf::iterateAccessor<fastgltf::math::fvec2>(asset, uvAccessor, [&](fastgltf::math::fvec2 t) {
	//			verticies[idx].uv[0] = t.data()[0];
	//			verticies[idx++].uv[1] = t.data()[1];
	//		});
	//		SubMesh subMesh(0, idx);
	//		subMeshes.emplace_back(subMesh);

	//	}
	//}
	//VertexBuffer vertexBuffer;
	//vertexBuffer.Init(nullptr, sizeof(Vertex), verticies.size(), verticies.data());

	//IndexBuffer indexBuffer;
	//indexBuffer.Init(nullptr, indicies.size(), indicies.data());

	//Mesh mesh(std::move(vertexBuffer), std::move(indexBuffer), std::move(subMeshes));

	//return mesh;

}