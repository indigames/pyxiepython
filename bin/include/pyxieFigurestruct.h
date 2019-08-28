///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////
#pragma once

#include <stdint.h>
namespace pyxie
{
	class pyxieTexture;

#define GEOM_VER 11

#define align64(v,n) union{v n; uint64_t n##64;}

	enum RenderPassFilter
	{
		ShadowPass = 1,		//影を作る
		OpaquePass = 2,		//不透明な
		//TestPass = 4,		//抜き
		TransparentPass = 4	//半透明な
	};


	struct SamplerState {
		uint16_t wrap_s : 3;
		uint16_t wrap_t : 3;
		uint16_t minfilter : 3;
		uint16_t magfilter : 3;
		uint16_t mipfilter : 3;
		uint16_t pad : 1;
		void Clear() { *((uint16_t*)this) = 0; }
		inline bool operator ==(const SamplerState & ss) { return *((uint16_t*)this) == *((uint16_t*)&ss); }
		inline bool operator !=(const SamplerState & ss) { return *((uint16_t*)this) != *((uint16_t*)&ss); }

		enum WrapMode {
			WRAP = 0,
			MIRROR,
			CLAMP,
			BORDER
		};
		enum FilterFunction {
			LINEAR = 0,
			NEAREST,
			NEAREST_MIPMAP_NEAREST,
			LINEAR_MIPMAP_NEAREST,
			NEAREST_MIPMAP_LINEAR,
			LINEAR_MIPMAP_LINEAR
		};
	};


	struct Sampler {
		align64(pyxieTexture*, tex);
		uint32_t textureNameIndex;
		SamplerState samplerState;
		uint16_t samplerSlot;
		inline bool operator ==(const Sampler & ss) { return ((uint64_t*)this)[0] == ((uint64_t*)&ss)[0] && ((uint64_t*)this)[1] == ((uint64_t*)&ss)[1]; }
		inline bool operator !=(const Sampler & ss) { return ((uint64_t*)this)[0] != ((uint64_t*)&ss)[0] || ((uint64_t*)this)[1] != ((uint64_t*)&ss)[1]; }
	};


	struct FigureMaterialParam {
		union {
			uint64_t iValue64[2];
			uint32_t iValue32[4];
			float fValue[4];
			Sampler sampler;
		};
		uint32_t hash;
		uint32_t type;
	};


	struct FigureMaterial{
		uint32_t shaderNameValue[8];
		uint32_t nameHash;
		uint8_t numParams;
		uint8_t numStates;
		uint8_t flags;
		uint8_t passFilter;
		FigureMaterialParam params[1];
		uint32_t size() { return 32 + 8 + sizeof(FigureMaterialParam)*(numParams + numStates); }
		enum
		{
			FlagSkin = 0x01,
			FlagMakeShadow = 0x02,
			FlagOpaquePath = 0x04
		};
	};


	struct EnvironmentSource {
		enum Flag {
			Enable = 1,
			Outsource = 2
		};
		enum Type {
			POINT = 0,
			SPOT,
			AMBIENT,
			DIRECTIONAL,
			FOG
		};
		uint32_t hash;
		uint16_t type;
		uint8_t flags;
		uint8_t no;
		align64(float*, transform);

		union {
			struct {
				float skyColor[4];
				float groundColor[4];
				float dir[4];
			}ambientLamp;
			struct {
				float color[4];
				float dir[4];
			}directionalLamp;
			struct {
				float color[4];
				float position[4];
			}pointLamp;
			struct {
				float color[4];
				float fognear;
				float fogfar;
				float fogalpha;
			}distanceFog;
		};
	};


	struct FigureCamera	// 80 bytes
	{
		align64(float*, transform);
		uint32_t nameHash;
		float fov;
		float aspect;
		float nearclip;
		float farclip;
		float magX;
		uint32_t isOrtho;
		uint32_t dummy;
	};


	struct FigureNotes
	{
		uint64_t numNotes;
		struct
		{
			uint32_t nameHash;
			uint32_t pad0;
			align64(char*,text);
		} note[1];
	};

	struct FigureEnvironment
	{
		enum
		{
			enable_backbuffer = 0x01,
			enable_shadow = 0x02,
			enable_manipulator = 0x04,
			enable_aspect = 0x08,
			enable_post_effect = 0x10,
			enable_color_correction = 0x11,
			enable_glare = 0x12
		};
		uint32_t		flags;
		float			blurLimit;
		float			blurOffset;
		float			blurScale;
		unsigned char	colorCorrection[256 * 4];
		float			bgColor[3];
	};


	enum AttributeID
	{
		ATTRIBUTE_ID_UNKNOWN = 0,
		ATTRIBUTE_ID_POSITION,
		ATTRIBUTE_ID_NORMAL,
		ATTRIBUTE_ID_TANGENT,
		ATTRIBUTE_ID_BINORMAL,
		ATTRIBUTE_ID_UV0,
		ATTRIBUTE_ID_UV1,
		ATTRIBUTE_ID_UV2,
		ATTRIBUTE_ID_UV3,
		ATTRIBUTE_ID_COLOR,
		ATTRIBUTE_ID_BLENDINDICES,
		ATTRIBUTE_ID_BLENDWEIGHT,
		ATTRIBUTE_ID_PSIZE,
		NUM_ATTRIBUTE_ID
	};

	struct VertexAttribute
	{
		uint8_t id;			//AttributeID
		uint8_t normalize;
		uint16_t size;		//Number of element 1～4
		uint16_t type;		//GL_FLOAT and so on
		uint16_t offset;	//location
	};


	struct FigureMesh	//64 bytes
	{
		align64(void*,vertices);
		align64(uint16_t*,indices);
		align64(uint32_t*,bonePaletteIndeces);
		align64(VertexAttribute*, vertexAttributes);
		//align64(FigureMaterial*, figureMaterial);
		uint32_t nameHash;
		uint32_t numVerticies;
		uint32_t numIndices;
		uint32_t material;
		int32_t verticesUid;	//unused
		int32_t indicesUid;		//
		uint16_t numSkinTransforms;
		uint16_t vertexFormatSize;
		uint16_t numVertexAttributes;
		uint8_t numWeightsPerVertex;
		uint8_t flags;

		enum MeshAttributeFlag{
			TRIANGLE_STRIP = 1,
			SKIP_RENDER = 2,
			WIREFRAME_MODE = 4,
			CONTINUES_BONEINDEX = 8,
			CLIP_TRANSPARENT = 16,
			SHADOW_MODE = 32
		};
	};


	struct FigureHeader	//88 bytes
	{
		uint32_t ver;
		uint16_t numMeshes;
		uint16_t numMaterials;
		uint16_t numEnvironmentSources;
		uint16_t numCameras;
		uint16_t numInbindPoses;
		uint16_t numParticles;
		uint16_t textureNameSize;
		uint16_t numAnimes;
		uint32_t pad0;
		align64(void*, skeleton);
		align64(uint64_t*, animations);
		align64(FigureMesh*,meshes);
		align64(float*, inbindPoses);
		align64(FigureMaterial*,mates);
		align64(char*, textureNames);
		align64(EnvironmentSource*,envsrc);
		align64(FigureCamera*,cameras);
		align64(void*, dummy);
	};


}