#pragma once
#include <string>

class MeshObjData {
public:
	MeshObjData() = default;

	void SetMesh(std::string meshIdent) { 
		this->meshIdent = meshIdent;
		this->materialidents.clear();
	}; // clear materialIdents
	void SetMaterial(size_t index, std::string materialIdent) {
		if (index >= this->materialidents.size() ) {
			std::string val;
			this->materialidents.insert(
				this->materialidents.end(), 
				this->materialidents.size() + 1 - index,
				val
			);
		}
		this->materialidents[index] = materialIdent;
	};
	std::string GetMeshIdent() { return this->meshIdent; }
	std::string GetMaterial(size_t index) { return index < this->materialidents.size() ? materialidents[index] : ""; }
private:
	std::string meshIdent;
	std::vector<std::string> materialidents;
};
