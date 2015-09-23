#include "IOUtil.h"

#include <stdlib.h>
#include <iostream>
#include <fstream>

bool ReadFile(const std::string& pFileName, std::string& outFile)
{
	std::ifstream f(pFileName);


	bool ret = false;

	if (f.is_open()) {
		std::string line;
		while (std::getline(f, line)) {
			outFile.append(line);
			outFile.append("\n");
		}

		f.close();

		ret = true;
	}
	else {

	}

	return ret;
}