///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////
#pragma once

#include "pyxie.h"
#include "pyxieMathutil.h"
#include "pyxieObject.h"
#include "pyxieTime.h"
#include "pyxieFigurestruct.h"

#define GL_ZERO                           0
#define GL_ONE                            1
#define GL_SRC_COLOR                      0x0300
#define GL_ONE_MINUS_SRC_COLOR            0x0301
#define GL_SRC_ALPHA                      0x0302
#define GL_ONE_MINUS_SRC_ALPHA            0x0303
#define GL_DST_ALPHA                      0x0304
#define GL_ONE_MINUS_DST_ALPHA            0x0305
#define GL_DST_COLOR                      0x0306
#define GL_ONE_MINUS_DST_COLOR            0x0307
#define GL_SRC_ALPHA_SATURATE             0x0308
//BlendingFactorSrc
#define GL_DST_COLOR                      0x0306
#define GL_ONE_MINUS_DST_COLOR            0x0307
#define GL_SRC_ALPHA_SATURATE             0x0308
//BlendingEquation
#define GL_FUNC_ADD                       0x8006
#define GL_MIN                            0x8007
#define GL_MAX                            0x8008
#define GL_FUNC_SUBTRACT                  0x800A
#define GL_FUNC_REVERSE_SUBTRACT          0x800B
#define GL_FRONT                          0x0404
#define GL_BACK                           0x0405
#define GL_NEVER                          0x0200
#define GL_LESS                           0x0201
#define GL_EQUAL                          0x0202
#define GL_LEQUAL                         0x0203
#define GL_GREATER                        0x0204
#define GL_NOTEQUAL                       0x0205
#define GL_GEQUAL                         0x0206
#define GL_ALWAYS                         0x0207
#define GL_ZERO                           0
#define GL_KEEP                           0x1E00
#define GL_REPLACE                        0x1E01
#define GL_INCR                           0x1E02
#define GL_DECR                           0x1E03
#define GL_INVERT                         0x150A
#define GL_INCR_WRAP                      0x8507
#define GL_DECR_WRAP                      0x8508
/* Boolean */
#define GL_TRUE                           1
#define GL_FALSE                          0
/* DataType */
#define GL_BYTE                         0x1400
#define GL_UNSIGNED_BYTE                0x1401
#define GL_SHORT                        0x1402
#define GL_UNSIGNED_SHORT               0x1403
#define GL_INT                          0x1404
#define GL_UNSIGNED_INT                 0x1405
#define GL_FLOAT                        0x1406
#define GL_2_BYTES                      0x1407
#define GL_3_BYTES                      0x1408
#define GL_4_BYTES                      0x1409
#define GL_DOUBLE                       0x140A
#define GL_HALF_FLOAT					0x140B

struct EdgeAnimPpuContext;

namespace pyxie
{
	class pyxieRenderTarget;
	class pyxieTexture;
	class pyxieShader;
	class pyxieCustomFigure;

	enum RenderStateParamType
	{
		None = 0,
		Int,
		Float
	};

	enum ParamInfoFlag {
		GlogalParam = 1,
		VPParam = 2,
		PackParam = 4
	};

	struct ShaderParameterInfo
	{
		char	name[32];
		void*	globalParamSourcePointer;
		uint32_t frame;
		uint32_t key;
		uint8_t	packID;
		uint8_t	packOffset;
		uint8_t	dataType;
		uint8_t	flags;
	};

	enum ShaderParameterKey
	{
		Key_Unknown = 0,
		Key_ViewProj = 1,
		Key_World = 2,
		Key_View = 3,
		Key_ViewInverse = 4,
		Key_WorldInverse = 5,
		Key_MatrixPalette = 6,
		Key_SystemAlpha = 7,
		Key_ViewportPixelSize = 8,
		Key_DirLamp01Color = 9,
		Key_DirLight01Intensity = 10,
		Key_DirLamp01Dir = 11,
		Key_DirLamp02Color = 12,
		Key_DirLight02Intensity = 13,
		Key_DirLamp02Dir = 14,
		Key_DirLamp03Color = 15,
		Key_DirLight03Intensity = 16,
		Key_DirLamp03Dir = 17,
		Key_PointLamp01Color = 18,
		Key_PointLight01Intensity = 19,
		Key_PointLight01Range = 20,
		Key_PointLamp01Pos = 21,
		Key_PointLamp02Color = 22,
		Key_PointLight02Intensity = 23,
		Key_PointLight02Range = 24,
		Key_PointLamp02Pos = 25,
		Key_PointLamp03Color = 26,
		Key_PointLight03Intensity = 27,
		Key_PointLight03Range = 28,
		Key_PointLamp03Pos = 29,
		Key_PointLamp04Color = 30,
		Key_PointLight04Intensity = 31,
		Key_PointLight04Range = 32,
		Key_PointLamp04Pos = 33,
		Key_PointLamp05Color = 34,
		Key_PointLight05Intensity = 35,
		Key_PointLight05Range = 36,
		Key_PointLamp05Pos = 37,
		Key_PointLamp06Color = 38,
		Key_PointLight06Intensity = 39,
		Key_PointLight06Range = 40,
		Key_PointLamp06Pos = 41,
		Key_PointLamp07Color = 42,
		Key_PointLight07Intensity = 43,
		Key_PointLight07Range = 44,
		Key_PointLamp07Pos = 45,
		Key_AmbientColor = 46,
		Key_SkyDir = 47,
		Key_SkyColor = 48,
		Key_GroundColor = 49,
		Key_ColorSampler = 50,
		Key_DiffuseColor = 51,
		Key_DiffuseAlpha = 52,
		Key_NormalSampler = 53,
		Key_Bumpiness = 54,
		Key_SpecularParams = 55,
		Key_SpecularStrength = 56,
		Key_SpecularPower = 57,
		Key_EnvSampler = 58,
		Key_ReflectionStrength = 59,
		Key_FresnelMin = 60,
		Key_FresnelExp = 61,
		Key_SurfaceColor = 62,
		Key_FuzzySpecColor = 63,
		Key_SubColor = 64,
		Key_RollOff = 65,
		Key_VelvetStrength = 66,
		Key_LightSampler = 67,
		Key_OverlayColorSampler = 68,
		Key_OverlayNormalSampler = 69,
		Key_OverlayBumpiness = 70,
		Key_OverlayStrength = 71,
		Key_CLUTSampler = 72,
		Key_BackBufferSampler = 73,
		Key_GlassParams = 74,
		Key_GlassStrength = 75,
		Key_Curvature = 76,
		Key_UpsideDown = 77,
		Key_ParallaxBias = 78,
		Key_ParallaxBiasX = 79,
		Key_ParallaxBiasY = 80,
		Key_ShadowSampler = 81,
		Key_LightViewProj = 82,
		Key_CameraPosition = 83,
		Key_Time = 84,
		Key_ScrollSpeedSet1 = 85,
		Key_ScrollSpeedSet1U = 86,
		Key_ScrollSpeedSet1V = 87,
		Key_ScrollSpeedSet2 = 88,
		Key_ScrollSpeedSet2U = 89,
		Key_ScrollSpeedSet2V = 90,
		Key_ScrollSpeedSet3 = 91,
		Key_ScrollSpeedSet3U = 92,
		Key_ScrollSpeedSet3V = 93,
		Key_OffsetSet1 = 94,
		Key_OffsetSet1U = 95,
		Key_OffsetSet1V = 96,
		Key_OffsetSet2 = 97,
		Key_OffsetSet2U = 98,
		Key_OffsetSet2V = 99,
		Key_DistortionStrength = 100,
		Key_FogParams = 101,
		Key_FogNear = 102,
		Key_FogFar = 103,
		Key_FogAlpha = 104,
		Key_FogColor = 105,
		Key_AlphaRef = 106,
		Key_CoverSampler = 107,
		Key_CoverScale = 108,
		Key_CoverScaleH = 109,
		Key_CoverScaleV = 110,
		Key_CoverAddition = 111,
		Key_MipBias = 112,
		Key_LightMapPower = 113,
		Key_LightMapStrength = 114,
		Key_Saturation = 115,
		Key_OffsetSet3 = 116,
		Key_OffsetSet3U = 117,
		Key_OffsetSet3V = 118,
		Key_Fat = 119,
		Key_RotationSet1 = 120,
		Key_RotationSet2 = 121,
		Key_RotationSet3 = 122,
		Key_______123 = 123,
		Key_______124 = 124,
		Key_______125 = 125,
		Key_______126 = 126,
		Key_______127 = 127,
		Key_______128 = 128,
		Key_ScaleSet1 = 129,
		Key_ScaleSet1U = 130,
		Key_ScaleSet1V = 131,
		Key_ScaleSet2 = 132,
		Key_ScaleSet2U = 133,
		Key_ScaleSet2V = 134,
		Key_ScaleSet3 = 135,
		Key_ScaleSet3U = 136,
		Key_ScaleSet3V = 137,
		Key_CoverOffset = 138,
		Key_CoverOffsetX = 139,
		Key_CoverOffsetY = 140,
		Key_ZBias = 141,
		Key_EnvsSampler = 142,
		Key_InnerGrowAValue = 143,
		Key_InnerGrowAPower = 144,
		Key_InnerGrowAStrength = 145,
		Key_InnerGrowALimit = 146,
		Key_GlowACLUTSampler = 147,
		Key_InnerGrowBValue = 148,
		Key_InnerGrowBPower = 149,
		Key_InnerGrowBStrength = 150,
		Key_InnerGrowBLimit = 151,
		Key_GlowBCLUTSampler = 152,
		Key_LocalIntensity = 153,
		Key_InnerGrowAColor = 154,
		Key_InnerGrowBColor = 155,
		Key_SepiaStrength = 156,
		Key_ViewOffset = 157,
		Key_ShadowColor = 158,
		Key_ColorSamplerScale = 159,
		Key_BloomLimit = 160,
		Key_BlurOffset = 161,
		Key_BloomScale = 162,
		Key_ShadowDensity = 163,
		Key_ColorCorrectionSampler = 164,
		Key_BlurredSampler = 165,
		Key_DepthSampler = 166,
		Key_DofParams = 167,
		Key_DofOptions = 168,
		Key_DofGrayScale = 169,
		Key_LightColor = 170,
		Key_MulColor = 171,
		Key_ScreenVertexSize = 172,
		Key_ColorTextureSize = 173,

		Key_alpha_func = 220,
		Key_alpha_test_enable = 221,
		Key_blend_func = 222,
		Key_blend_equation = 223,
		Key_blend_enable = 224,
		Key_cull_face = 225,
		Key_cull_face_enable = 226,
		Key_depth_func = 227,
		Key_depth_mask = 228,
		Key_depth_test_enable = 229,
		Key_polygon_offset = 230,
		Key_polygon_offset_fill_enable = 231,
		Key_color_mask = 232,
		Key_stencil_func = 233,
		Key_stencil_mask = 234,
		Key_stencil_op = 235,
		Key_stencil_test_enable = 236,
		Key_scissor_test_enable = 237,
		Key_scissor = 238
	};

	struct RenderStateInfo
	{
		char				name[32];
		char				paramName1[16];
		char				paramName2[16];
		char				paramName3[16];
		char				paramName4[16];
		ShaderParameterKey	id;
		uint32_t			defaultvalue[4];
		uint32_t			defaultvalueBackup[4];
		uint32_t			lastvalue[4];
		char				valueTypes[4];
		char				valueCount;
		bool				enableOnShadow;
	};

	class PYXIE_EXPORT pyxieRenderContext : public pyxieObject
	{
	protected:
		Mat4		world;
		Mat4		view;
		Mat4		proj;
		Mat4		viewProj;
		Mat4		invView;
		Mat4		invWorld;
		Mat4		tview;
		Mat4		tproj;
		Mat4		tviewProj;
		Mat4		tInvView;
		Mat4		tshadowView;
		Mat4		shadowView;
		Vec4		cameraPos;

		static const int numDirLithts = 3;
		static const int numPointLithts = 7;
		Vec3		dirLight[numDirLithts];
		Vec3		dirLightCol[numDirLithts];
		Vec4		pointLight[numPointLithts];
		Vec4		pointLightCol[numPointLithts];

		//float		totalTime;
		//TimeVal	startTime;
		Vec3		ambientColor;
		Vec3		skyDir;
		Vec3		groundColor;
		Vec3		fogParams;
		Vec3		fogColor;
		float		mipBias;
		float		shadowDensity;

		uint32_t	currentPassFilter;
		uint32_t	numChangedRenderStates;
		pyxieRenderTarget*	currentRenderTarget;


		pyxieTexture*		defaultShadowTexture;
		pyxieTexture*		dummyTexture;
		Sampler			shadowTexture;
		Sampler			backBufferTexture;
		Sampler			coverTexture;
		Vec3			coverScale;
		Vec3			coverOffset;
		Vec3			shadowColor;

		pyxieShader*		lastRenderedShader;

		int renderCounter;
		int renderStateCounter;

		EdgeAnimPpuContext*	animeContext;
		void* externalSpuStorage;

		uint32_t paramFrame;

		Sampler activeSampler[6];

	public:

		~pyxieRenderContext();

		void SetActiveSampler(Sampler& sampler);

		///ディレクショナルライトパラメータをセット
		void SetDirectionalLight(int no, const Vec3& dir, float intensity, const Vec3& col);

		inline const Vec3& GetDirectionalLightColor(int no){
			return dirLightCol[no];
		}

		inline const Vec3& GetDirectionalLightDir(int no){
			return dirLight[no];
		}

		///ポイントライトパラメータをセット
		void SetPointLight(int no, const Vec3& pos, float intensity, const Vec3& col, float range);


		///ポイントライトのカラーを取得(カラーにはintensityが乗算されている)
		inline const Vec4& GetPointLightColor(int no) { return pointLightCol[no]; }

		///ポイントライトの位置を取得（wにライトの有効範囲が入っている）
		inline const Vec4& GetPointLightPosition(int no) { return pointLight[no]; }

		///アンビエントライトパラメータをセット
		void SetAmbientLight(const Vec3& col);

		//半球アンビエントライトパラメータをセット
		void SetHemisphereLight(const Vec3& dir, const Vec3& skyCol, const Vec3& groundCol);

		//アンビエントライトカラーを取得
		inline const Vec3& GetAmbientLightColor(){ return ambientColor;}

		//半球アンビエントのグランドカラーを取得
		inline const Vec3& GetHemisphereLightGroundColor(){return groundColor;}

		//半球アンビエントのディレクションを取得
		inline const Vec3& GetHemisphereLightDirection(){return skyDir;}

		///フォグパラメータをセット
		void SetFog(float fogNear, float fogFar, float fogAlpha, const Vec3& color);

		///フォグカラーを取得
		inline const Vec3& GetFogColor(){return fogColor;}

		///フォグアルファ（フォグの強さ：0～1）を取得
		inline float GetFogAlpha(){return fogParams.Z();}

		///影色をセット
		void SetShadowColor(const Vec3& color);

		///影色の取得
		inline const Vec3& GetShadowColor(){return shadowColor;}

		///ミップマップバイアスの値を取得する
		///（現在のところiOSでだけ反映される値です）
		inline float GetMipmapBias() { return mipBias; }

		///ミップマップバイアスの値を取得する
		///（現在のところiOSでだけ反映される値です）
		inline void SetMipmapBias(float bias) { mipBias = bias; }

		///現在レンダリング先に指定されているRenderTargetを取得
		///NULLの場合はディスプレイ
		inline pyxieRenderTarget* GetCurrentRenderTarget() { return currentRenderTarget; }

		void BeginScene(pyxieRenderTarget* renderTarget, const Vec4& clearColor = Vec4(0.4f, 0.6f, 0.8f, 1.0f), bool clearColorBuffer = true, bool clearDepthBuffer = true);
		void BeginPass(uint32_t passID);
		void EndScene();

		///シャドウバッファをセットする
		void SetShadowTexture(pyxieTexture* tex);

		///バックバッファをセットする
		void SetBackBufferTexture(pyxieTexture* tex);

		///カバーテクスチャをセットする
		void SetCoverTexture(pyxieTexture* tex);

		///カバーテクスチャスケールをセットする
		inline void SetCoverScale(float xScale, float zScale) { coverScale = Vec3(1.0f / xScale, 1.0f / zScale, 0); }

		///カバーテクスチャオフセットをセットする
		inline void SetCoverOffset(float xOfs, float zOfs) { coverOffset = Vec3(xOfs, zOfs, 0); }

		///レンダーステートのデフォルト値を変更する。
		///value=NULLにすると起動時の値に戻る
		void SetDefaultRenderState(ShaderParameterKey key, void* value = NULL);

		void ResetRenderStateAll();
		void RenderStateUpdateStart();
		void RenderStateUpdate(ShaderParameterKey key, void* value);
		void RenderStateUpdateEnd();

		RenderStateInfo* GetRenderStateInfo(ShaderParameterKey key);

		///影生成レンダリング時にもセットする必要のあるレンダーステートかどうかを返す
		bool IsUpdateRenderStateOnShadowPass(ShaderParameterKey key);

		void SetMipBias(float bias);

		///シャドウマッピングの影の濃さ
		//shadowDensity	0:アンビエント色～1:見えない薄さ（マイナスの値にすると、アンビエント以上に濃い色になる）
		void SetShadowDensity(float v);
		float GetShadowDensity() { return shadowDensity; }

		//シーン全体のフレームレートを設定する
		//_fps : フレームレート（デフォルト60）
		//_fix : 固定フレームレートにする）
		void SetTargetFPS(int _fps, bool _fix = true);

		//シーン全体のフレームレートを取得する
		int GetTargetFPS();
		bool IsFixFrameRate();

		//プロファイル関連

		//レンダリングされたメッシュの数を数える
		void ResetMeshCounter();
		int GetMeshCounter();

		//変更されたレンダーステートの回数を数える
		void ResetRenderStateCounter();
		int GetRenderStateCounter();

		//最後にセットされたトランスフォームの取得
		inline const Mat4& GetRenderViewMatrix() { return view; }
		inline const Mat4& GetRenderProjectionMatrix() { return proj; }
		inline const Mat4& GetRenderViewProjectionMatrix() { return viewProj; }

		inline uint32_t GetCurrentPassFilter() { return currentPassFilter; }

		void SetViewProjection(const Mat4& invViewMatrix, const Mat4& projMatrix);
		void SetViewProjection(const Mat4& invViewMatrix, float fov, float nearPlane, float farPlane, bool orthographic, const Mat4& screenMatrix, float aspect = -1.0f);
		void SetShadowViewProjection(const Mat4& invViewMatrix, float fov, float nearPlane, float farPlane);

		const ShaderParameterInfo* GetShaderParameterInfoByName(const char* name);
		const ShaderParameterInfo* GetShaderParameterInfoByHash(uint32_t hash);
		const ShaderParameterInfo* GetShaderParameterInfoByKey(uint32_t key);

		uint32_t GetMaterialParamValueCount(uint32_t key);
		uint32_t GetNumShaderParameterInfo();

		bool BackupShader(pyxieShader* shader) {
			if (shader != lastRenderedShader)
			{
				lastRenderedShader = shader;
				return true;
			}
			return false;
		}


		static void New(DeviceHandle handle);
		static void Delete();
		static pyxieRenderContext& Instance();

		EdgeAnimPpuContext* GetAnimeContext();
		void* GetAnimeBuffer();
		uint32_t AnimeBufferSize();

	private:
		pyxieRenderContext(DeviceHandle handle);

		void SetRenderState(ShaderParameterKey key, const void* value);
		void ConvertXYWH01toPixel(const float* xywh_in, int* xywh_out, pyxieRenderTarget* rt);

		void UpdateGrovalParamFrame() { paramFrame++; }

		bool PlatformInit(DeviceHandle handle);
		void PlatformTerminate();

	};
}