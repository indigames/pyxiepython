///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////
#pragma once
#include "pyxieDrawable.h"
#include "pyxieFigurestruct.h"
#include <vector>
#include <map>

namespace pyxie
{
	class pyxieAnimator;
	class pyxieShader;
	class pyxieEnvironmentSet;
	class pyxieCamera;

	struct MeshInfo {
		pyxieShader* shader;
		Vec4 color;
	};


	class PYXIE_EXPORT pyxieFigure : public pyxieDrawable
	{
		enum {
			state_Posed = (state_resource_last <<1),		//A state in which the function Pose() is called at least once.
			state_HoldJointTransfrom = (state_Posed << 1)	//Hold transform of skeleton joint after pause calculation.
		};

	public:
		//７つのスロットにAnimatorをセットして、以下の演算によって最終的な姿勢を決定する。
		//FinalPose = (PartA0 * PartA1) + (PartB0 * PartB1) + (PartC0 * PartC1) + Base (+:合成 , *:リニア補完)
		//Base以外の全てのモーションはBaseからの差分モーションを指定する
		enum AnimatorSlot
		{
			SlotBase = 0,
			SlotA0,
			SlotA1,
			SlotB0,
			SlotB1,
			SlotC0,
			SlotC1,
			NumSlot
		};

		pyxieFigure(const char* path);
		pyxieFigure(pyxieFigure* org);
		~pyxieFigure();

		void Build();
		void Initialize();
		void Clone(bool afterFinishBuild);
		RESOURCETYPE ResourceType() { return FIGURETYPE; }

		void Dump(const char* file);
			
		virtual void Pose();
		virtual void Render();
		void Step(float elapsedTime);
		void SetEvalTime(float time);

		pyxieEnvironmentSet* GetEnvironmentSet();
		pyxieCamera* GetCamera(const char* name=nullptr);

		int GetJointIndex(uint32_t jointHash);

		//Animatorを関連付ける
		//alot     :  登録先のアニメーションスロット
		//animator :  
		void BindAnimator(AnimatorSlot slot, pyxieAnimator* animator);
		void BindAnimator(AnimatorSlot slot, const char* name);



		//アニメーションスロットに関連付けられたAnimatorを取得する
		inline pyxieAnimator* GetBindedAnimator(AnimatorSlot slot) { return animatorSlot[slot]; }

		//A0-A1, B0-B1, C0-C1間のモーション線形補間係数を設定（取得）する
		//ABC	 : 0=PartA, 1=PartB, 2=PartC
		//weight : 0～1
		inline void SetBlendingWeight(uint32_t ABC, float weight) { blendWeight[ABC] = weight; }
		inline float GetBlendingWeight(uint32_t ABC) { return blendWeight[ABC]; }

		FigureHeader* GetHeader() { return header; }

		

	protected:
		FigureHeader* header;
		pyxieAnimator* animatorSlot[NumSlot];
		float	lastEvalTime[NumSlot];
		pyxieShader** meshShaders;

		float blendWeight[3];
		uint32_t brendTreeType;
		void* blendTreeBranches;
		void* blendTreeLeaves;
		uint16_t numBlendBranches;
		uint16_t numBlendLeaves;

		float* skinningMatrices;
		float* inbindSkinningMatrices;

		Transform*	localJointBuffer;

		float* userChannelAnimeBuffer;

		std::map<uint32_t, pyxieAnimator*> animes;

	private:
		void ClearMember();
		void CreateBlendTree();
		void CopyBlendWeight();
		bool BuildMesh();
		void DeleteMesh();
		void DrawMesh(FigureMesh& mesh, pyxieShader* shader);
		bool LoopMesh(int& i);
	};
}