#include "scene/objectFromStringFactory.h"

void* ObjectFromStringFactory::Construct(std::string const& n)
{
	MapType::iterator i = classes.find(n);
	
	if (i == classes.end()) 
	{
		Logger::Log("name, ", n.c_str());
		throw std::runtime_error("Failed to construct class. You made up that class name.");
	}

	return i->second();
}
