#pragma once

#include "utilities/logger.h"

class Test {
public:
	Test() = default;
	~Test() = default;

	void TestFunc() {
		Logger::log("Test 2");
	}
};