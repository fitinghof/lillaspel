#include "gameObjects/objectLoader.h"
#include "utilities/logger.h"
#include <fastgltf/core.hpp>
#include <fastgltf/tools.hpp>
#include <fastgltf/util.hpp>
#include <unordered_map>


ObjectLoader::ObjectLoader() {
}

ObjectLoader::~ObjectLoader() {
}

bool ObjectLoader::LoadGltf(Mesh& mesh, std::filesystem::path path, ID3D11Device* device) {
	fastgltf::Parser parser;

	auto gltfFile = fastgltf::GltfDataBuffer::FromPath(path);
	if (gltfFile.error() != fastgltf::Error::None) {
		Logger::Error("Failed to load gltf file, error: ");
		return false;
	}

	constexpr auto gltfOptions =
		fastgltf::Options::DontRequireValidAssetMember |
		fastgltf::Options::AllowDouble |
		fastgltf::Options::LoadExternalBuffers |
		fastgltf::Options::LoadExternalImages |
		fastgltf::Options::GenerateMeshIndices;

	auto data = parser.loadGltf(gltfFile.get(), path.parent_path(), gltfOptions);

	if (data.error() != fastgltf::Error::None) {
		Logger::Error("Failed to load glTF file, error: ");
		return false;
	}
	auto& asset = data.get();

	std::unordered_map<uint32_t, uint32_t> bufferOffsets;
	uint32_t totalOffset = 0;
	std::vector<Vertex> verticies;
	std::vector<uint32_t> indices;
	uint32_t indexOffset = 0;

	for (auto& gltfmesh : asset.meshes) {

		std::vector<SubMesh> submeshes;
		for (auto it = gltfmesh.primitives.begin(); it != gltfmesh.primitives.end(); ++it) {

			auto* positionIt = it->findAttribute("POSITION");
			assert(positionIt != it->attributes.end());
			assert(it->indicesAccessor.has_value());

			// If we haven't processed these vertexes yet, do so.
			if (bufferOffsets.find(positionIt->accessorIndex) == bufferOffsets.end()) {
				bufferOffsets[positionIt->accessorIndex] = totalOffset;
				auto& positionAccessor = asset.accessors[positionIt->accessorIndex];

				verticies.reserve(verticies.size() + positionAccessor.count);
				Logger::Log("Number of positions: ", positionAccessor.count);

				// Wacky for loop
				fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec3>(asset, positionAccessor, [&](fastgltf::math::fvec3 pos, std::size_t idx) {

					verticies.push_back(Vertex{});

					verticies[totalOffset + idx].pos[0] = pos.x();
					verticies[totalOffset + idx].pos[1] = pos.y();
					verticies[totalOffset + idx].pos[2] = pos.z();

					verticies[totalOffset + idx].uv[0] = 0;
					verticies[totalOffset + idx].uv[1] = 0;
					});

				totalOffset += static_cast<uint32_t>(positionAccessor.count * sizeof(Vertex));

				auto* normalIt = it->findAttribute("NORMAL");
				if (normalIt != it->attributes.end()) {
					auto& normalAccessor = asset.accessors[normalIt->accessorIndex];
					// Wacky for loop
					fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec3>(asset, normalAccessor, [&](fastgltf::math::fvec3 normal, std::size_t idx) {
						verticies[bufferOffsets[positionIt->accessorIndex] + idx].normal[0] = normal.x();
						verticies[bufferOffsets[positionIt->accessorIndex] + idx].normal[1] = normal.y();
						verticies[bufferOffsets[positionIt->accessorIndex] + idx].normal[2] = normal.z();
						});
				}
				else {
					// Implement normal generation later
					Logger::Warn("No normals found for mesh primitive!");
				}
			}

			//Add uv processing

			
			// Parse Indicies
			fastgltf::Accessor& indexAccessor = asset.accessors[it->indicesAccessor.value()];

			// Should never happen due to load options
			if (!indexAccessor.bufferViewIndex.has_value()) {
				Logger::Error("No indexAccessor was found, this should not happen :(");
				return false;
			}
				

			// reserve memory
			indices.reserve(indices.size() + indexAccessor.count);

			// initialize memory
			indices.insert(indices.end(), indexAccessor.count, 0);
			if (indexAccessor.componentType == fastgltf::ComponentType::UnsignedByte || indexAccessor.componentType == fastgltf::ComponentType::UnsignedShort) {
				std::vector<uint16_t> temp(indexAccessor.count);
				fastgltf::copyFromAccessor<std::uint16_t>(asset, indexAccessor, temp.data());
				for (size_t index = 0; index < temp.size(); index++) {
					indices[indexOffset + index] = temp[index];
				}
			}
			else {
				fastgltf::copyFromAccessor<std::uint32_t>(asset, indexAccessor, indices.data() + indexOffset);
			}

			if (it->materialIndex.has_value()) {
				// Apply material
			}

			// Create the index buffer and copy the indices into it.
			submeshes.emplace_back(indexOffset, indexAccessor.count);

			// Increment indexOffset for next submesh
			indexOffset += static_cast<uint32_t>(indexAccessor.count);

		}

		VertexBuffer vertexBuffer;
		vertexBuffer.Init(device, sizeof(Vertex), static_cast<UINT>(verticies.size()), verticies.data());

		IndexBuffer indexBuffer;
		indexBuffer.Init(device, indices.size(), indices.data());

		mesh.Init(std::move(vertexBuffer), std::move(indexBuffer), std::move(submeshes));
	}

	return true;
	
}