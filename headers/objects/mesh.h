#pragma once
#include <string>
#include <array>
#include <vector>
#include "rendering/vertex.h"

class SubMesh {
public:


private:
	std::string name;
	std::vector<Vertex> primitives;
};

class Mesh {
public:


private:
	std::string name;
	std::vector<SubMesh> subMeshes;
};
