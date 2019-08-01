#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <vector>

//#include "pyxieTypes.h"
#ifdef PYXIE_SHARED
#ifdef _WIN32
#define PYXIE_EXPORT __declspec(dllexport)
#else
#define PYXIE_EXPORT
#endif
#else
#define PYXIE_EXPORT
#endif


namespace pyxie {

	struct ShaderSourceParam {
		std::string name;
		std::string type;
	};

	class PYXIE_EXPORT pyxieShaderGen
	{
		std::string target;
	public:
		pyxieShaderGen() {}
		void Setup(const char* targetType, std::stringstream& outputStringStream);
		void Precompile(const std::string& instr, std::stringstream& outputStringStream);
		void GetParams(const std::string& code, std::vector<ShaderSourceParam>& outList, const char* paramType);
	};

}
