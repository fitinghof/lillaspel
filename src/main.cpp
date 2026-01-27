#include <iostream>
#include "test.h"
#include "utilities/logger.h"

// Main code
int main(int)
{
    Logger::Log("This is a normal log.");
    Logger::Warn("This is a warning.");
    Logger::Error("This is an error.");
}
