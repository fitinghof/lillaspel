#pragma once
#define NOMINMAX //tells windows to not define min/max so I can use algorithm min/max instead
#include <algorithm>
#include <d3d11.h>
#include "core/tools.h"

struct SATData
{
	DirectX::XMFLOAT3 center;
	DirectX::XMFLOAT3* positionData;
	size_t nrOfPositions;
	DirectX::XMFLOAT3* normalData;
	size_t nrOfNormals;
};

struct PhysicsMaterial
{
	float bounce = 0;
	float friction = 0.5f;
};

inline float OverlapOnAxis(const SATData& shapeA, const SATData& shapeB, DirectX::FXMVECTOR axis);
bool SAT(const SATData& shapeA, const SATData& shapeB, DirectX::XMFLOAT3& resolveAxis, float& resolveDistance);