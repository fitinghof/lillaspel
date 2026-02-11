#pragma once 
#include <array>
struct Ray {
	std::array<float, 3> origin;
	std::array<float, 3> direction;
};

class RayCaster {
public:
	RayCaster();
	~RayCaster();

private:
};
