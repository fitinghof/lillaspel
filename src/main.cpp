#include <iostream>
#include "test.h"
#include "utilities/logger.h"

// Main code
int main(int)
{
    Logger::log("This is a normal log.");
    Logger::warn("This is a warning.");
    Logger::error("This is an error.");
}
