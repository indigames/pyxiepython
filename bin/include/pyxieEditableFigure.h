///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////
#pragma once
#include "pyxieDrawable.h"
#include "pyxieShader.h"
#include "pyxieTexture.h"
#include "pyxieMakeShape.h"
#include "pyxieShaderDescriptor.h"
#include "pyxieMemorywriter.h"
#include "pyxieBinaryFileHelper.h"

#include <stdlib.h>
#include <string.h>
#include <map>
#include <set>
#include <vector>

namespace pyxie
{
	struct TextureSource {
		char path[MAX_PATH];
		bool normal;
		bool wrap;
	};

	struct UserChannelInfo{
		unsigned int        m_nodeNameHash;
		unsigned int        m_channelNameHash;
		unsigned char       m_componentIndex;
		unsigned char       m_flags;

		//std::string			nodeName;
	};

	struct HierachyQuad
	{
		int m_index[4];
		int m_indexParent[4];
	};

	class Skeleton{
	public:
		unsigned int                    m_numJoints;
		unsigned int                    m_numUserChannels;
		std::vector<int>                m_parentIndices;
		std::vector<Joint>              m_basePose;
		std::vector<bool>               m_scaleCompensateFlags;
		std::vector<unsigned int>       m_jointNameHashes;
		std::vector<UserChannelInfo>    m_userChannelInfoArray;
		std::vector<std::string>		m_jointNames;
		Skeleton() { m_numJoints = 0;  m_numUserChannels = 0; }
		~Skeleton() { }

		void Clear() {
			m_parentIndices.clear();
			m_basePose.clear();
			m_scaleCompensateFlags.clear();
			m_jointNameHashes.clear();
			m_userChannelInfoArray.clear();
			m_jointNames.clear();
			m_numJoints = 0; 
			m_numUserChannels = 0;
		}
	};


	// Default global error tolerance
	static const float  kDefaultTolerance = 0.0003f;

	// Default sample rate for non-sampled animation data
	// Already sampled data should use its own sample rate
	static const float	kDefaultSampleRate = 30.0f;

	//--------------------------------------------------------------------------------------------------

	struct JointFrameSet
	{
		Vec4 m_initialRData;
		Vec4 m_initialTData;
		Vec4 m_initialSData;

		unsigned int m_intraRFrame0;                // index of first intra rotation keyframe
		unsigned int m_intraTFrame0;                // index of first intra translation keyframe
		unsigned int m_intraSFrame0;                // index of first intra scale keyframe

		unsigned int m_numIntraRFrames;
		unsigned int m_numIntraTFrames;
		unsigned int m_numIntraSFrames;

		std::vector<unsigned char> m_hasRFrame;     // for each intra frame, 1 if this joint has a rotation keyframe, or 0
		std::vector<unsigned char> m_hasTFrame;     // for each intra frame, 1 if this joint has a translation keyframe, or 0
		std::vector<unsigned char> m_hasSFrame;     // for each intra frame, 1 if this joint has a scale keyframe, or 0
	};

	struct UserChannelFrameSet
	{
		float m_initialData;

		unsigned int m_intraFrame0;                 // index of first intra keyframe
		unsigned int m_numIntraFrames;

		std::vector<unsigned char> m_hasFrame;      // for each intra frame, 1 if this channel has a keyframe, or 0
	};

	struct FrameSet
	{
		unsigned int m_baseFrame;
		unsigned int m_numIntraFrames;

		std::vector<JointFrameSet> m_jointFrameSets;
		std::vector<UserChannelFrameSet> m_userChannelFrameSets;
	};

	struct AnimationKeyframe
	{
		float   m_keyTime;  // keyframe time
		Vec4  m_keyData;  // keyframe data
	};

	struct JointAnimation
	{

		unsigned int                    m_jointName;            // name hash of joint
		float                           m_jointWeight;          // for partial animations / feathering

		std::vector<AnimationKeyframe>  m_rotationAnimation;
		std::vector<AnimationKeyframe>  m_translationAnimation;
		std::vector<AnimationKeyframe>  m_scaleAnimation;
	};

	struct UserChannelAnimation
	{
		unsigned int                    m_nodeName;             // name hash of node
		unsigned int                    m_channelName;          // name hash of channel
		float                           m_weight;               // for partial animations / feathering

		std::vector<AnimationKeyframe>  m_animation;

		//std::string						nodeName;
	};

	class Animation
	{
	public:
		float                               m_startTime;                // start time
		float                               m_endTime;                  // end time
		float                               m_period;                   // sampling frequency
		unsigned int                        m_numFrames;                // number of frames

		std::vector<JointAnimation>         m_jointAnimations;
		std::vector<UserChannelAnimation>   m_userChannelAnimations;
		uint32_t							m_nameID;

		Animation() { m_numFrames = 0; }
		~Animation() {}

		void Clear() {
			m_numFrames = 0;
			for (auto it = m_jointAnimations.begin(); it != m_jointAnimations.end(); it++) {
				it->m_rotationAnimation.clear();
				it->m_translationAnimation.clear();
				it->m_scaleAnimation.clear();
			}
			for (auto it = m_userChannelAnimations.begin(); it != m_userChannelAnimations.end(); it++) {
				it->m_animation.clear();
			}
			m_jointAnimations.clear();
			m_userChannelAnimations.clear();
			m_nameID = 0;
		}
	};

	enum EdgeAnimCompressionType
	{
		COMPRESSION_TYPE_NONE,				// uncompressed floats (non-rotations only)
		COMPRESSION_TYPE_SMALLEST_3,		// 48 bit quaternions (rotations only)
		COMPRESSION_TYPE_BITPACKED			// arbitrary per-component bit length
	};

	struct CompressionInfo
	{
		EdgeAnimCompressionType	m_compressionTypeRotation;
		EdgeAnimCompressionType	m_compressionTypeTranslation;
		EdgeAnimCompressionType	m_compressionTypeScale;
		EdgeAnimCompressionType	m_compressionTypeUser;

		float					m_defaultToleranceRotation;			// default tolerance used by COMPRESSION_TYPE_BITPACKED
		float					m_defaultToleranceTranslation;		// default tolerance used by COMPRESSION_TYPE_BITPACKED
		float					m_defaultToleranceScale;			// default tolerance used by COMPRESSION_TYPE_BITPACKED
		float					m_defaultToleranceUser;				// default tolerance used by COMPRESSION_TYPE_BITPACKED

		std::vector<float>		m_jointTolerancesRotation;			// optional per-joint tolerances (if empty, default is used)
		std::vector<float>		m_jointTolerancesTranslation;		// optional per-joint tolerances (if empty, default is used)
		std::vector<float>		m_jointTolerancesScale;				// optional per-joint tolerances (if empty, default is used)
		std::vector<float>		m_userChannelTolerances;			// optional per-user channel tolerances (if empty, default is used)

		CompressionInfo() :
			m_compressionTypeRotation(COMPRESSION_TYPE_SMALLEST_3),
			m_compressionTypeTranslation(COMPRESSION_TYPE_NONE),
			m_compressionTypeScale(COMPRESSION_TYPE_NONE),
			m_compressionTypeUser(COMPRESSION_TYPE_NONE),
			m_defaultToleranceRotation(kDefaultTolerance),
			m_defaultToleranceTranslation(kDefaultTolerance),
			m_defaultToleranceScale(kDefaultTolerance),
			m_defaultToleranceUser(kDefaultTolerance)
		{}
	};


	struct DrawSet {
		int ofs;
		int size;
		std::vector<FigureMaterialParam>* params;
		std::vector<FigureMaterialParam>* states;
	};

	struct EditableMesh {
		uint32_t nameHash;
		uint32_t materialNameHash;

		uint32_t numVertexAttributes;
		VertexAttribute* attributes;

		uint32_t numFloatsPerVertex;
		uint32_t vertexBufferSize;
		uint32_t numVertices;
		float* vertices;

		uint32_t indexBufferSize;
		uint32_t numTriangles;
		uint32_t* triangles;
		uint32_t indexSize;
		uint32_t primitiveType;

		bool skipRender;
		bool continuesIndexes;
		uint32_t numTryStrips;
		uint32_t* tryStrips;
		uint32_t numTransforms;
		int32_t* GPUTransformIndexes;
		uint32_t maxSkinWeightsPerVertex;
		bool outsource;
		std::vector<DrawSet>* drawSet;
		int vboNo;
		bool enablevbo;
	};

	struct VBO
	{
		bool reset;
		EditableMesh* mesh;
		uint32_t vbo[2];
		uint32_t vao;
	};



	typedef void(*CustomDataCallback) (pyxieMemoryWriter& writer, void* customData);

	class ColladaLoader;

	class PYXIE_EXPORT pyxieEditableFigure : public pyxieDrawable {
		//void* skeletonset;
		void* baseAnimeset;
		//float* skinningMatrices;
		//float* inbindSkinningMatrices;
		std::vector<VBO> vbos;
		std::vector<std::string> jointNames;
	protected:
		std::vector<EnvironmentSource> figureEnvironments;
		std::vector<FigureCamera> figureCameras;
		std::map<uint32_t, FigureMaterial*> figureMaterials;
		//std::map<uint32_t, EditableMesh*> editableMeshes;
		std::vector<EditableMesh*> editableMeshes;
		std::vector<TextureSource> textures;
		Skeleton* skeleton;
		Animation* baseAnimation;
		std::vector<Animation*> additiveAnimation;
		std::vector<std::string> additiveAnimationName;
		Mat3x4*	inverseBindMatrixes;
		bool resetSkeletonSet;

	public:
		pyxieEditableFigure(const char* name);
		pyxieEditableFigure(pyxieEditableFigure* org);
		~pyxieEditableFigure();

		void Build();
		void Initialize();
		void Clone(bool afterFinishBuild);
		RESOURCETYPE ResourceType() { return FIGURETYPE; }
			
		virtual void Pose();
		virtual void Render();

		//add elemet
		bool AddMaterial(const char* materialName, pyxieShaderDescriptor& desc);
		bool AddMesh(const char* nodeName, const char* materialName);
		bool AddJoint(int parentIndex, Joint& pose, bool scaleCompensate, const char* jointName);	//parentIndex == -1 if root

		//cont elemet
		int NumMeshes() { return editableMeshes.size(); }
		int NumMaterials() { return figureMaterials.size(); }
		int NumJoints() { return skeleton ? skeleton->m_numJoints : 0; }

		//edit mesh
		int GetMeshIndex(uint32_t meshNameHash);
		bool SetMeshVertexAttributes(int index, VertexAttribute* attr, uint32_t numAttr);
		bool SetMeshVertices(int index, const void* ptr, uint32_t numVerts);
		bool SetMeshVertexValues(int index, const void* ptr, uint32_t numVerts, AttributeID attr, uint32_t startPosition, uint32_t align = 16);
		bool SetMeshIndices(int index, uint32_t startPosition, const uint32_t* triangles, uint32_t numTriangles, uint32_t datasize, uint32_t align = 16);
		void SetMeshPrimitiveType(int index, uint32_t type);
		bool AddMeshDrawSet(int index, int offset, int size);
		bool AddMeshDrawSetState(int index, int setNo, uint32_t key, void* value);
		void CalcMeshAABBox(int index, float* outmin, float* outmax);
		EditableMesh* GetMesh(int index);
		bool MergeMesh();

		//edit joint
		int GetJointIndex(uint32_t jointHash);
		const Joint GetJoint(int index);
		const void  SetJoint(int index, const Joint& joint);

		// edit material
		bool SetMaterialParam(const char* materialName, const char* paramName, const void* value, ShaderParameterDataType dataType = ParamTypeUnknown);
		bool GetMaterialParam(const char* materialName, const char* paramName, void* value);
		bool SetMaterialState(const char* materialName, uint32_t key, void* value);

		// save figure
		bool SaveFigure(const char* path, bool excludeBaseAnime = false, bool excludeSkeleton = false);
		bool SaveAnimation(const char* path);
		bool SaveSkeleton(const char* path);

		// texture source
		const std::vector<TextureSource>& GetTextureSources() { return textures; }
		uint32_t SetTextureSource(const TextureSource& texturesource);
		void ReplaceTextureSource(const TextureSource& oldTexture, const TextureSource& newTexture);

		void ClearAll();
		void ClearAllMeshes();

		static const VertexAttribute& GetVertexAttribute(AttributeID id);


#if defined __ENABLE_SUSPEND_RECOVER__
		virtual bool Restore();
		virtual bool Release();
#endif
	protected:
		// save figure
protected:
		void ConvertGPUSkinningScene(uint32_t bonePaletteSize);
		bool ExportSkeleton(pyxieMemostream* pStream, const Skeleton* skeleton, CustomDataCallback customDataCallback, void* customData);
		bool BuildSimdHierarchy(std::vector<HierachyQuad>& hierachyQuads, const Skeleton* skeleton, const std::set<int>& boneSet);
		void DrawMesh(EditableMesh* mesh, pyxieShader* shader);

    void SaveMeshes(FigureHeader* figureHeader, pyxieBinaryFileHelper& fileHelper);
		void SaveInbindPoses(FigureHeader* figureHeader, pyxieBinaryFileHelper& fileHelper);
		void SaveMaterials(FigureHeader* figureHeader, pyxieBinaryFileHelper& fileHelper);
		void SaveNames(FigureHeader* figureHeader, pyxieBinaryFileHelper& fileHelper);
		void SaveLights(FigureHeader* figureHeader, pyxieBinaryFileHelper& fileHelper);
		void SaveCameras(FigureHeader* figureHeader, pyxieBinaryFileHelper& fileHelper);


		void ExtendEditableMesh(EditableMesh* mesh, const FigureMaterial* oldmate, const FigureMaterial* newmate);


		void ResetVertexBuffers();
		int NewVBO();

		void ClearMesh(EditableMesh* emesh);

		friend class pyxieColladaLoader;
	};

}
