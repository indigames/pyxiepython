///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////
#pragma once

typedef int CGparameter;

#include "pyxieResource.h"
#include "pyxieFigurestruct.h"

namespace pyxie
{
	struct ShaderParameterInfo;

	
	struct AlphaBlendingInfo
	{
		uint32_t	BlendEnable;
		uint32_t	BlendFuncSrc;
		uint32_t	BlendFuncDest;
		uint32_t	BlendEquation;
		uint32_t	TestEnable;
		uint32_t	AlphaEquation;
		float		AlphaValue;
	};

	struct ShaderParameter {
		FigureMaterialParam param;
		const ShaderParameterInfo* info;
		uint32_t valueFrame;
		char name[32];
		CGparameter cgParam;
	};

	struct ParamTypeName {
		char name[12];
		ShaderParameterDataType type;
	};

	class PYXIE_EXPORT pyxieShader : public pyxieResource
	{
		char* _vpo_start;
		char* _fpo_start;
		int _vpo_size;
		int _fpo_size;
		CGparameter attributeLocation[NUM_ATTRIBUTE_ID];
		ShaderParameter* globalParameters;
		ShaderParameter* localParameters;
		uint32_t* shaderAttributes;
		int numGlobalParameters;
		int numLocalParameters;
		int numShaderAttributes;
		uint32_t pass;
		uint32_t vertShader;
		uint32_t fragShader;
		uint32_t program;
		uint32_t matrixPalette;

		//AlphaBlendingInfo		alphaBlendingInfo;
		//uint32_t alphaRefPos;
		static const char* attributeNames[NUM_ATTRIBUTE_ID];
	public:
		pyxieShader(const char* path);
		pyxieShader(pyxieShader* org);
		~pyxieShader();

		void Build();
		void Initialize();
		void Clone(bool afterFinishBuild);
		RESOURCETYPE ResourceType() { return SHADERTYPE; }

		void Render();

		void SetLocalParameter(FigureMaterialParam& mateparam);
		void SetMatrixPalette(uint32_t num, const float* value);

		inline bool Pass(uint32_t filter){
			return (filter & pass) ? true : false;
		}

		CGparameter GetAttributeLocation(AttributeID id){
			return attributeLocation[id]; 
		}

		inline const ShaderParameter* GetLocalParameters(int& numLocalParams) {
			numLocalParams = 0;
			if (!WaitBuild()) return nullptr;
			numLocalParams = numLocalParameters;
			return localParameters;
		}

		inline const ShaderParameter* GetGlobalParameters(int& numGlobalParams) {
			numGlobalParams = 0;
			if (!WaitBuild()) return nullptr;
			numGlobalParams = numGlobalParameters;
			return globalParameters;
		}

		inline const uint32_t* GetShaderAttributes(int& numAttrs) {
			numAttrs = 0;
			if (!WaitBuild()) return nullptr;
			numAttrs = numShaderAttributes; return shaderAttributes;
		}
		static void AutoSaveShader(const char* dir);
		static void AutoReadShader(const char* dir);

		//inline const AlphaBlendingInfo* GetAlphaBlendingInfo() { return &alphaBlendingInfo; }
#if defined __ENABLE_SUSPEND_RECOVER__
		virtual bool Restore();
		virtual bool Release();
#endif
private:
		void ClearMember();
		void SetParameter(CGparameter location, uint32_t paramtype, void* value);
		uint32_t VertexAttributeNameToType(const char* name);
		ShaderParameterDataType ShaderParameterDataNameToType(const char* typeName);
	};

}
