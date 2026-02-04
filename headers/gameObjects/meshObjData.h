#pragma once
#include <string>

class MeshObjData {
public:
	MeshObjData() = default;

	void SetMesh(std::string meshIdent) {}; // clear materialIdents
	void SetMaterial(size_t index, std::string materialIdent) {};
private:
	std::string meshIdent;
	std::vector<std::string> materialidents;
};