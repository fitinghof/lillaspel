#include "gameObjects/objectLoader.h"
#include "utilities/logger.h"
#include <fastgltf/core.hpp>
#include <fastgltf/tools.hpp>
#include <fastgltf/util.hpp>


ObjectLoader::ObjectLoader() {
}

ObjectLoader::~ObjectLoader() {
}

bool ObjectLoader::LoadGltf(Mesh& mesh, std::filesystem::path path, ID3D11Device* device) {
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
	for (auto& gltfmesh : asset.meshes) {

		std::vector<SubMesh> submeshes;
		for (auto it = gltfmesh.primitives.begin(); it != gltfmesh.primitives.end(); ++it) {
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
			indexBuffer.Init(nullptr, indices.size(), indices.data());

			submeshes.emplace_back(
				std::move(vertexBuffer),
				std::move(indexBuffer)   
			);

		}
		mesh.Init(std::move(submeshes));
	}

	return true;
	
}