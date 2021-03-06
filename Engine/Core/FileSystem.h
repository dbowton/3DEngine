#pragma once
#include <string>

namespace dwb
{
	void SetFilePath(const std::string& pathname);
	std::string GetFilePath();

	bool ReadFileToString(const std::string& filename, std::string& filestring);
}