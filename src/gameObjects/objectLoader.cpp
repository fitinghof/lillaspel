#include "gameObjects/objectLoader.h"
#include "utilities/logger.h"
#include <fastgltf/core.hpp>
#include <fastgltf/tools.hpp>
#include <fastgltf/util.hpp>
#include <unordered_map>
#include <rendering/texture.h>
#include <WICTextureLoader.h>
#include <unordered_set>


ObjectLoader::ObjectLoader(std::filesystem::path basePath): basePath(std::move(basePath)) {
}

ObjectLoader::~ObjectLoader() {
}

bool ObjectLoader::LoadGltf(std::filesystem::path localpath, MeshLoadData& meshLoadData, ID3D11Device* device) {
	std::filesystem::path path = basePath / localpath;
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

	std::unordered_map<uint32_t, Texture> loadedTextures;
	std::unordered_map<std::string, Material> materials;
	size_t meshIndex = 0;
	
	for (auto& gltfmesh : asset.meshes) {

		std::vector<Vertex> verticies;
		uint32_t totalOffset = 0;
		std::vector<uint32_t> indices;
		uint32_t indexOffset = 0;
		std::unordered_map<uint32_t, uint32_t> bufferOffsets;
		std::vector<SubMesh> submeshes;

		Mesh mesh;

		mesh.SetName(path.generic_string() + ":Mesh_" + std::to_string(meshIndex));
		

		MeshObjData data;
		data.SetMesh(mesh.GetName());

		for (auto it = gltfmesh.primitives.begin(); it != gltfmesh.primitives.end(); ++it) {

			auto* positionIt = it->findAttribute("POSITION");
			assert(positionIt != it->attributes.end());
			assert(it->indicesAccessor.has_value());
			
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView(nullptr);

			
			// If we haven't processed these vertexes yet, do so.
			if (bufferOffsets.find(positionIt->accessorIndex) == bufferOffsets.end()) {
				bufferOffsets.emplace(positionIt->accessorIndex, totalOffset);

				size_t oldOffset = totalOffset;
				
				this->LoadVerticiesAndNormals(asset, *it, verticies, totalOffset);
				
				bool success = this->LoadUV(asset, *it, verticies, oldOffset);
				if (!success) {
					Logger::Error("Failed to load UV");
					return false;
				}


			}

			uint32_t indexStart = indexOffset;
			if (!this->LoadIndices(asset, *it, indices, indexOffset)) {
				Logger::Error("Loading indices failed");
				return false;
			}				

			submeshes.emplace_back(indexStart, indexOffset - indexStart);
			

			auto& material = asset.materials[it->materialIndex.value_or(0)];

			auto& baseColorTexture = material.pbrData.baseColorTexture;

			Material materialOut;

			
			if (baseColorTexture.has_value() && asset.textures[baseColorTexture->textureIndex].imageIndex.has_value()) {
				auto& texture = asset.textures[baseColorTexture->textureIndex];
				auto textureNameIt = loadedTextures.find(texture.imageIndex.value());
				if (textureNameIt == loadedTextures.end()) {
					auto* textureRaw = this->LoadTexture(asset, * it, device);
					if (textureRaw == nullptr) {
						return false;
					}
					std::string texIdent = path.generic_string() + ":Tex_" + std::to_string(loadedTextures.size());
					Texture tex(textureRaw,  texIdent);

					loadedTextures.emplace(texture.imageIndex.value(), tex);
					materialOut.textures.emplace_back(new Texture(tex));
				}
				else {
					materialOut.textures.emplace_back(new Texture(loadedTextures.at(texture.imageIndex.value())));
				}
			}
			
			
			// Extract more material stuff

			std::string materialIdent = path.generic_string() + ":Mat_" + std::to_string(materials.size());
			materialOut.identifier = materialIdent;

			data.SetMaterial(materials.size(), materialIdent);
			materials.emplace(materialIdent, std::move(materialOut));

		}
		VertexBuffer vertexBuffer;
		vertexBuffer.Init(device, sizeof(Vertex), static_cast<UINT>(verticies.size()), verticies.data());

		IndexBuffer indexBuffer;
		indexBuffer.Init(device, indices.size(), indices.data());

		mesh.Init(std::move(vertexBuffer), std::move(indexBuffer), std::move(submeshes));

		meshLoadData.meshes.emplace_back(std::move(mesh));
		meshLoadData.meshData.emplace_back(data);
		meshIndex++;
	}
	

	return true;
	
}

void ObjectLoader::LoadVerticiesAndNormals(const fastgltf::Asset& asset, const fastgltf::Primitive& primitive, std::vector<Vertex>& dest, uint32_t& offset)
{
	auto* positionIt = primitive.findAttribute("POSITION");
	assert(positionIt != primitive.attributes.end());
	assert(primitive.indicesAccessor.has_value());

	auto& positionAccessor = asset.accessors[positionIt->accessorIndex];

	dest.reserve(dest.size() + positionAccessor.count);

	// Wacky for loop
	fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec3>(asset, positionAccessor, [&](fastgltf::math::fvec3 pos, std::size_t idx) {

		dest.push_back(Vertex{});

		dest[offset + idx].pos[0] = pos.x();
		dest[offset + idx].pos[1] = pos.y();
		dest[offset + idx].pos[2] = pos.z();

		dest[offset + idx].uv[0] = 0;
		dest[offset + idx].uv[1] = 0;
	});


	auto* normalIt = primitive.findAttribute("NORMAL");
	if (normalIt != primitive.attributes.end()) {
		auto& normalAccessor = asset.accessors[normalIt->accessorIndex];
		// Wacky for loop
		fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec3>(asset, normalAccessor, [&](fastgltf::math::fvec3 normal, std::size_t idx) {
			dest[offset + idx].normal[0] = normal.x();
			dest[offset + idx].normal[1] = normal.y();
			dest[offset + idx].normal[2] = normal.z();
			});
	}
	else {
		// Implement normal generation later maybe
		Logger::Warn("No normals found for mesh primitive!");
	}
	offset += static_cast<uint32_t>(positionAccessor.count);
}

bool ObjectLoader::LoadIndices(fastgltf::Asset& asset, const fastgltf::Primitive& primitive, std::vector<uint32_t>& dest, uint32_t& offset)
{
	Logger::Log("Loading indices");
	// Parse Indicies
	fastgltf::Accessor& indexAccessor = asset.accessors[primitive.indicesAccessor.value()];

	// Should never happen due to load options
	if (!indexAccessor.bufferViewIndex.has_value()) {
		Logger::Error("No indexAccessor was found, this should not happen :(");
		return false;
	}


	// reserve memory
	dest.reserve(dest.size() + indexAccessor.count);

	// initialize memory
	dest.insert(dest.end(), indexAccessor.count, 0);
	if (indexAccessor.componentType == fastgltf::ComponentType::UnsignedByte || indexAccessor.componentType == fastgltf::ComponentType::UnsignedShort) {
		std::vector<uint16_t> temp(indexAccessor.count);
		fastgltf::copyFromAccessor<std::uint16_t>(asset, indexAccessor, temp.data());
		for (size_t index = 0; index < temp.size(); index++) {
			dest[offset + index] = temp[index];
		}
	}
	else {
		fastgltf::copyFromAccessor<std::uint32_t>(asset, indexAccessor, dest.data() + offset);
	}
	offset += static_cast<uint32_t>(indexAccessor.count);
	Logger::Log("Loaded indices");
	return true;
}

bool ObjectLoader::LoadUV(const fastgltf::Asset& asset, const fastgltf::Primitive& primitive, std::vector<Vertex>& dest, size_t offset)
{
	size_t baseColorTextureCordIndex = 0;
	auto& material = asset.materials[primitive.materialIndex.value_or(0)];

	auto& baseColorTexture = material.pbrData.baseColorTexture;
	if (baseColorTexture.has_value()) {
		auto& texture = asset.textures[baseColorTexture->textureIndex];
		if (!texture.imageIndex.has_value()) {
			Logger::Error("Texture has no image index!");
			return false;
		}
		
		if (baseColorTexture->transform && baseColorTexture->transform->texCoordIndex.has_value()) {
			baseColorTextureCordIndex = baseColorTexture->transform->texCoordIndex.value();
		}
		else {
			baseColorTextureCordIndex = material.pbrData.baseColorTexture->texCoordIndex;
		}
	}

	auto texCordAttrName = "TEXCOORD_" + std::to_string(baseColorTextureCordIndex);
	if (const auto* texcoord = primitive.findAttribute(texCordAttrName); texcoord != primitive.attributes.end()) {
		// Get texcord accessor
		auto& texCoordAccessor = asset.accessors[texcoord->accessorIndex];
		if (!texCoordAccessor.bufferViewIndex.has_value())
			return true;

		fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec2>(asset, texCoordAccessor, [&](fastgltf::math::fvec2 uv, std::size_t idx) {
			dest[offset + idx].uv[0] = uv.x();
			dest[offset + idx].uv[1] = uv.y();
		});
	}
	return true;
}

ID3D11ShaderResourceView* ObjectLoader::LoadTexture(const fastgltf::Asset& asset, const fastgltf::Primitive& primitive, ID3D11Device* device)
{
	Logger::Log("Loading textures");
	ID3D11ShaderResourceView* textureView = nullptr;
	size_t baseColorTextureCordIndex = 0;
	auto& material = asset.materials[primitive.materialIndex.value_or(0)];

	auto& baseColorTexture = material.pbrData.baseColorTexture;
	if (baseColorTexture.has_value()) {
		auto& texture = asset.textures[baseColorTexture->textureIndex];
		if (!texture.imageIndex.has_value()) {
			Logger::Error("Texture has no image index!");
			return nullptr;
		}
		auto& textureImage = asset.images[texture.imageIndex.value()];
		std::visit(fastgltf::visitor{
			[](auto& arg) {},
			[&](fastgltf::sources::URI& filePath) {
				// maybe implement?
			},
			[&](fastgltf::sources::Array& vector) {
				// maybe implement?
			},
			[&](fastgltf::sources::BufferView& view) {
				auto& bufferView = asset.bufferViews[view.bufferViewIndex];
				auto& buffer = asset.buffers[bufferView.bufferIndex];

				std::visit(fastgltf::visitor {

					[](auto& arg) {},
					[&](fastgltf::sources::Array& vector) {
						int width, height, nrChannels;
						DirectX::CreateWICTextureFromMemory(
							device,
							reinterpret_cast<const uint8_t*>(vector.bytes.data() + bufferView.byteOffset),
							static_cast<size_t>(bufferView.byteLength),
							nullptr,
							&textureView
						);

					}
				}, buffer.data);
			},
		}, textureImage.data);
	}
	Logger::Log("Loaded texture");
	return textureView;
}
