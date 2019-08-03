#pragma once
#include <string>
#include <sstream>
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

	class PYXIE_EXPORT pyxieShaderDescriptor
	{
	public:
		enum
		{
			Disable = 0,
			Enable
		};

		enum TextureWrapMode {
			REPEAT = 0,
			CLAMP,
			MIRROR
		};

		enum ReferenceMapChannel {
			NoMapChannel = 0,
			DiffuseMapAlphaChannel,		//d
			DiffuseMapRedChannel,		//
			NormalMapAlphaChannel,		//n
			NormalMapRedChannel,		//
			LightMapAlphaChannel,		//
			LightMapRedChannel,			//
			VertexColorRedChanel,		//c
			VertexColorAlphaChanel		//a
		};

		enum AmbientType {
			None = 0,
			Ambient,
			Hemisphere
		};

		enum AlphaBlendOP {
			COL = 0,
			ADD,
			SUB,
			MUL,
		};

		enum Space {
			LocalSpace = 0,
			WorldSpace,
			ViewSpace,
			ProjectionSpace
		};
	private:

		struct _ShaderDesc
		{
			static 	constexpr int buffersize = 8;
			union {
				struct {
					uint32_t SKINNING_ENABLE : 1;
					uint32_t BONE_INFLUENCE : 3;					//0,1,2,3,4
					uint32_t CALC_BINORMAL : 1;						//_, B
					//uint32_t ANDROID_SHADER : 1;					//_, A
					uint32_t NUM_DIR_LAMP : 2;						//0,1,2,3
					uint32_t NUM_POINT_LAMP : 3;					//0,1,2,3,4,5,6,7
					uint32_t AMBIENT_TYPE : 2;						//_, H, A	None, HemiSphere Ambient
					uint32_t CLUT_LIGHT_ENABLE : 1;					//_, C
					uint32_t SPECULAR_ENABLE : 1;					//_, S
					uint32_t USE_SPECULAR_MAP_CHANNEL : 4;			//_, n,d,c,a
					uint32_t USE_AMBIENT_OCCLUSION_CHANNEL : 4;		//_, n,d,c,a
					uint32_t VERTEX_DIFFUSE_COLOR_ENABLE : 1;		//_, C
					uint32_t COLOR_MAP_ENABLE : 1;					//_, D
					uint32_t NORMAL_MAP_ENABLE : 1;					//_, N
					uint32_t LIGHT_MAP_ENABLE : 1;					//_, L
					uint32_t OVERLAY_COLOR_MAP_ENABLE : 1;			//_, d
					uint32_t OVERLAY_NORMAL_MAP_ENABLE : 1;			//_, n
					uint32_t OVERLAY_SPECULAR_MAP_ENABLE : 1;		//_, a
					uint32_t OVERLAY_VERTEX_ALPHA_ENABLE : 1;		//_, v
					uint32_t colorMapUVSet : 4;						//0,1,2,3,4,5,6,7,8
					uint32_t normalMapUVSet : 4;					//0,1,2,3,4,5,6,7,8
					uint32_t lightMapUVSet : 4;						//0,1,2,3,4,5,6,7,8
					uint32_t overlayColorMapUVSet : 4;				//0,1,2,3,4,5,6,7,8
					uint32_t overlayNormalMapUVSet : 4;				//0,1,2,3,4,5,6,7,8
					uint32_t REFLECTION_ENABLE : 1;					//_, R
					uint32_t FRESNEL_ENABLE : 1;					//_, f
					uint32_t VELVET_ENABLE : 1;						//_, V
					uint32_t GLASS_MAP_ENABLE : 1;					//_, G
					uint32_t ALPHA_BLENDING_ENABLE : 1;				//_, B					*
					uint32_t VERTEX_ALPHA_ENABLE : 1;				//_, A
					uint32_t ALPHA_BLEND_OP : 2;					//_, a, s, m, c			*
					uint32_t DOUBLE_SIDE : 1;						//_, D					*
					uint32_t Z_REFERENCE_ENABLE : 1;				//_, Z					*
					uint32_t Z_WRITE_ENABLE : 1;					//_, W					*
					uint32_t COLOR_MASK_ENABLE : 1;
					uint32_t STENCIL_ENABLE : 1;
					uint32_t PARALLAX_MAP_ENABLE : 1;				//_, P
					uint32_t HEIGHT_MAP_CHANNEL : 4;				//_, n,d
					uint32_t MAKE_SHADOW_ENABLE : 1;				//_, S
					uint32_t RECEIVE_SHADOW_ENABLE : 1;				//_, S
					uint32_t POINT_PARTICLE_SHADER : 1;				//_, P
					uint32_t COVER_MAP_ENABLE : 1;					//
					uint32_t COVER_MAP_SPACE : 2;					//
					uint32_t ALPHA_TEST_ENABLE : 1;					//_, T
					uint32_t POINT_SAMPLING : 1;					//_, p					*
					uint32_t UVSCROLL_SET1 : 1;						//_, 1
					uint32_t UVSCROLL_SET2 : 1;						//_, 2
					uint32_t UVSCROLL_SET3 : 1;						//_, 3
					uint32_t UVOFFSET_SET1 : 1;						//_, 4
					uint32_t OL_PARALLAX_MAP_ENABLE : 1;			//_, P
					uint32_t OL_HEIGHT_MAP_CHANNEL : 4;				//_, n,d
					uint32_t UVOFFSET_SET2 : 1;						//_, 5
					uint32_t UVOFFSET_SET3 : 1;						//_, 6
					uint32_t BILLBOARD : 1;							//_, B
					uint32_t YLOCK : 1;								//_, Y
					uint32_t FOG : 1;								//_, F
					uint32_t HEIGHT_FOG : 1;						//_, h
					uint32_t FOG_MAP : 1;							//_, P
					uint32_t FOG_MAP_CHANNEL : 4;					//_, n,d
					uint32_t COLOR_SAMPLER_WRAP_MODE_U : 2;			//				*
					uint32_t COLOR_SAMPLER_WRAP_MODE_V : 2;			//				*
					uint32_t NORMAL_SAMPLER_WRAP_MODE_U : 2;		//				*
					uint32_t NORMAL_SAMPLER_WRAP_MODE_V : 2;		//				*
					uint32_t LIGHT_SAMPLER_WRAP_MODE_U : 2;			//				*
					uint32_t LIGHT_SAMPLER_WRAP_MODE_V : 2;			//				*
					uint32_t OVERLAY_COLOR_SAMPLER_WRAP_MODE_U : 2;	//				*
					uint32_t OVERLAY_COLOR_SAMPLER_WRAP_MODE_V : 2;	//				*
					uint32_t OVERLAY_NORMAL_SAMPLER_WRAP_MODE_U : 2;	//			*
					uint32_t OVERLAY_NORMAL_SAMPLER_WRAP_MODE_V : 2;	//			*
					uint32_t ALPHA_MAP_ENABLE : 1;					//_, C
					uint32_t ALPHA_CHANNEL : 4;						//_,d,n,l,r
					uint32_t BRIGHTNESS_ENABLE : 1;					//_,B
					uint32_t DIFFUSE_STR_MAP_ENABLE : 1;			//_, D
					uint32_t DIFFUSE_STR_MAP_CHANNEL : 4;			//_,d,n,l,r
					uint32_t SATURATION_ENABLE : 1;					//_,S
					uint32_t FAT_ENABLE : 1;						//_,F
					uint32_t OPAQUE_PATH : 1;						//_,O
					uint32_t UVROTATION_SET1 : 1;					//_,1
					uint32_t UVROTATION_SET2 : 1;					//_,2
					uint32_t UVROTATION_SET3 : 1;					//_,3
					uint32_t UVSCALE_SET1 : 1;						//_,1
					uint32_t UVSCALE_SET2 : 1;						//_,2
					uint32_t UVSCALE_SET3 : 1;						//_,3
					uint32_t PROJ_MAP0 : 1;							//_,0
					uint32_t PROJ_MAP1 : 1;							//_,1
					uint32_t PROJ_MAP2 : 1;							//_,2
					uint32_t ZBIAS : 1;								//_,Z
					uint32_t LIGHT_CALC : 2;						//
					uint32_t ENV_SPHERE : 1;						//
					uint32_t GLOW_A : 1;							//
					uint32_t GLOW_A_SUB : 1;						//
					uint32_t GLOW_A_MAP : 1;						//
					uint32_t GLOW_B : 1;							//
					uint32_t GLOW_B_SUB : 1;						//
					uint32_t GLOW_B_MAP : 1;						//
					uint32_t DISTORTION_ENABLE : 1;					//
					uint32_t DISTORTION_SOURCE_OVERRAY : 1;			//_,d
					uint32_t NORMAL_DISTORTION_ENABLE : 1;			//_,D
					uint32_t LOCAL_INTENSITY : 1;					//_,I
					uint32_t DIFFUSE_LIGHT_MAP_CHANNEL : 4;			//_,d,n,l,r
					uint32_t REFLECTION_ALPHA : 1;					//_,R
					uint32_t FAT_PAINT : 1;							//_,F
					uint32_t SEPIA_ENABLE : 1;						//_,S
					uint32_t GLOW_OFFSET : 1;						//_,o
					uint32_t OVERLAY_COLOR_MAP_ADD : 1;				//_,a
				};
				uint32_t buffer[buffersize];
			};
		};
		struct _ShaderInterpolationInfo
		{
			static 	constexpr int buffersize = 8;
			union {
				struct {
					uint32_t TIME_ENABLE : 1;
					uint32_t SAMPLER_ENABLE : 1;
					uint32_t COLOR_SAMPLER_ENABLE : 1;
					uint32_t NORMAL_SAMPLER_ENABLE : 1;
					uint32_t LIGHT_SAMPLER_ENABLE : 1;
					uint32_t OVERLAY_COLOR_SAMPLER_ENABLE : 1;
					uint32_t OVERLAY_NORMAL_SAMPLER_ENABLE : 1;
					uint32_t COVER_SAMPLER_ENABLE : 1;
					uint32_t CLUT_MAP_ENABLE : 1;
					uint32_t TRANSPARENT_MAP_CHANNEL : 4;
					uint32_t OVERLAY_SPECULAR_MAP_CHANNEL : 4;
					uint32_t OVERLAY_TRANSPARENT_MAP_CHANNEL : 4;
					uint32_t OVERLAY_STRENGTH_ENABLE : 1;
					uint32_t VERTEX_PAINT_ENABLE : 1;
					uint32_t ALPHA_BLEND_STATE_ENABLE : 1;
					uint32_t VIEW_MATRIX : 1;
					uint32_t VIEW_OFFSET : 1;
					uint32_t LOCAL_SPACE_VIEW : 1;
					uint32_t NOLIGHT : 1;
					uint32_t VIEW_INVERSE_MATRIX : 1;
					uint32_t USE_TEX0 : 1;
					uint32_t USE_TEX1 : 1;
					uint32_t USE_TEX2 : 1;
					uint32_t USE_WPOS : 1;
					uint32_t USE_COUV : 1;
					uint32_t USE_NOM : 1;
					uint32_t USE_TAN : 1;
					uint32_t USE_BIN : 1;
					uint32_t USE_SPOS : 1;
					uint32_t USE_VIEW : 1;
					uint32_t USE_VIEW2 : 1;
					uint32_t USE_SHDW : 1;
					uint32_t USE_FOG : 1;
					uint32_t CMAP_UV : 3;
					uint32_t CMAPC_UV : 3;
					uint32_t CMAPA_UV : 3;
					uint32_t NMAP_UV : 3;
					uint32_t NMAPC_UV : 3;
					uint32_t NMAPA_UV : 3;
					uint32_t LMAP_UV : 3;
					uint32_t LMAPC_UV : 3;
					uint32_t LMAPA_UV : 3;
					uint32_t OCMAP_UV : 3;
					uint32_t OCMAPC_UV : 3;
					uint32_t OCMAPA_UV : 3;
					uint32_t ONMAP_UV : 3;
					uint32_t ONMAPC_UV : 3;
					uint32_t ONMAPA_UV : 3;
					uint32_t NOM_VEC : 3;
					uint32_t TEX0_VEC : 3;
					uint32_t FOG_FACTOR : 3;
					uint32_t SPECCONTRIB : 1;
					uint32_t VERTEX_COLOR_ENABLE : 1;
				};
				uint32_t buffer[8];
			};
		};
		_ShaderDesc desc;
		_ShaderInterpolationInfo info;
	public:
		pyxieShaderDescriptor() {
			for (int i = 0; i < _ShaderDesc::buffersize; i++) desc.buffer[i] = 0;
			for (int i = 0; i < _ShaderInterpolationInfo::buffersize; i++) info.buffer[i] = 0;
		}
		void SetValue(const uint32_t* val);
		void SetName(const char* Name);
		std::string GetName();
		const uint32_t* GetValue() { return desc.buffer; }
		uint32_t GetValueSize() { return _ShaderDesc::buffersize * sizeof(uint32_t); }
		bool SetOldStyleName(const char* oldStyleName);
		void WriteShaderDefinition(const char* targetType, std::stringstream& outputStringStream);
		void ResetStateInfo();
		void InterpolationDesc();
		void GetAttributeID(std::vector<uint32_t>& outIDs);
		void SetBoneCondition(int numBoneINfluence, int totalBone);
		void SetCalcBinormalInShader(bool enable);
		void SetNumDirLamp(int num);
		void SetNumPointLamp(int num);
		void SetAmbientType(int type);
		void SetClutLamp(bool enable);
		void SetSpecular(bool enable, ReferenceMapChannel textureChannel);
		void SetAmbientOcclusion(ReferenceMapChannel textureChannel);
		void SetVertexColor(bool enable);
		void SetVertexAlpha(bool enable);
		void SetColorTexture(bool enable);
		void SetNormalTexture(bool enable);
		void SetLightTexture(bool enable);
		void SetOverlayColorTexture(bool enable);
		void SetOverlayNormalTexture(bool enable);
		void SetOverlaySpecularTexture(bool enable);
		void SetOverlayVertexAlpha(bool enable);
		void SetColorTextureUVChannel(int rgb, int a);
		void SetNormalTextureUVChannel(int rgb, int a);
		void SetLightTextureUVChannel(int rgb, int a);
		void SetOverlayColorTextureUVChannel(int rgb, int a);
		void SetOverlayNormalColorTextureUVChannel(int rgb, int a);

		static inline bool CompareShaderValue(const uint32_t* name1, const uint32_t* name2) {
			for (int i = 0; i < 4; i++)
				if (((uint64_t*)name1)[i] != ((uint64_t*)name2)[i]) return false;
			return true;
		}
	};

}