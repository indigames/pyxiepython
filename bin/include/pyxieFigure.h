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
		uint32_t verticesUid;
		uint32_t indicesUid;
		uint32_t vao;
		//Vec4 color;
	};


	class PYXIE_EXPORT pyxieFigure : public pyxieDrawable
	{
		enum {
			state_HoldJointTransfrom = (state_drawable_last << 1)	//Hold transform of skeleton joint after pause calculation.
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
		inline void SetBlendingWeight(uint32_t ABC, float weight) 
		{ 
			if (!vmath_almostEqual(blendWeight[ABC], weight)) {
				SetState(state_updateTransform);
				ResetState(state_posed);
			}
			blendWeight[ABC] = weight; 
		}
		inline float GetBlendingWeight(uint32_t ABC) { return blendWeight[ABC]; }

		FigureHeader* GetHeader() { return header; }

		bool SetMaterialParam(const char* materialName, const char* paramName, void* value);
		bool GetMaterialParam(const char* materialName, const char* paramName, void* value);

/*
		///アルファ値
		void SetAlpha(float a) { color.setW(a); }
		float GetAlpha() { return color.getW(); }
		//メッシュ単位で透明度を指定する
		//
		//name :ノード名
		//alpha:透明度
		///※非同期読込完了までロックする
		void	SetMeshAlpha(const char* name, float alpha);
		float	GetMeshAlpha(const char* name);

		//メッシュ単位で表示／非表示を指定する
		//初期化が終わっていない場合適応されない
		//
		//name :ノード名（NULLの場合全てのメッシュに適応する）
		//visible: 可視／不可視
		///※非同期読込完了までロックする
		void	SetVisible(const char* name, bool visible);
		bool	GetVisible(const char* name);


		//メッシュ単位でレンダリングモード（ワイヤフレーム／ポリゴン）を指定する
		//初期化が終わっていない場合適応されない
		//
		//name :ノード名（NULLの場合全てのメッシュに適応する）
		//wire: ワイヤフレーム／ポリゴン
		///※非同期読込完了までロックする
		void	SetWireFrame(const char* name, bool wire);
		bool	GetWireFrame(const char* name);

		///ルートのトランスフォームをMatrix4に変換して返す
		inline Matrix4 GetWorldTransform()
		{
			Matrix4 mat;
			JointsToMatrix4(mat, &rootJoint);
			return mat;
		}

		void UpdateInbindTransform();

		///マテリアルに関連付けられているパラメータとレンダーステートの内容を書き換える
		///materialnameをNULLにすると、すべてのマテリアルを書き換えの対象にする
		///※非同期読込に対応。初期化が終わっていない場合、コマンドをキャッシュし、初期化完了後にセットする
		bool SetMaterialParamValue(const char* materialname, ShaderParameterKey key, const void* value);

		///マテリアルに関連付けられているパラメータとレンダーステートの内容を取得する
		///※非同期読込完了までロックする
		bool GetMaterialParamValue(const char* materialname, ShaderParameterKey key, void* value);

		///メッシュに含まれる全てのマテリアルのパラメータとレンダーステートの内容を書き換える
		///※非同期読込に対応。初期化が終わっていない場合、コマンドをキャッシュし、初期化完了後にセットする
		bool SetMaterialParamValueByTransform(const char* name, ShaderParameterKey key, const void* value);

		///ストランスフォームのインデックスを使うバージョン
		///※非同期読込完了までロックする
		bool SetMaterialParamValueByTransformIndex(int idx, ShaderParameterKey key, const void* value);


		///メッシュに関連付けられている最初に見つかったマテリアルのパラメータとレンダーステートの内容を取得する
		///※非同期読込完了までロックする
		bool GetMaterialParamValueByTransform(const char* name, ShaderParameterKey key, void* value);
		bool GetMaterialParamValueByTransformIndex(int idx, ShaderParameterKey key, void* value);


		///シーン全体のAABBox（メッシュ単位のAABBoxの総和）を取得する
		///※アニメーションによる変形は考慮されていない
		///※非同期読込完了までロックする
		inline AABBox GetAABBox()
		{
			bool fin = IsFinishBuild();
			if (fin == false) return AABBox();
			return AABBox(geomHeader->bBoxCenter[0] - geomHeader->bBoxHalfSize[0], geomHeader->bBoxCenter[1] - geomHeader->bBoxHalfSize[1], geomHeader->bBoxCenter[2] - geomHeader->bBoxHalfSize[2],
				geomHeader->bBoxCenter[0] + geomHeader->bBoxHalfSize[0], geomHeader->bBoxCenter[1] + geomHeader->bBoxHalfSize[1], geomHeader->bBoxCenter[2] + geomHeader->bBoxHalfSize[2]);
		}

		///ノード名を使って、メッシュごとのAABBoxを取得する
		///存在しない名前を指定すると体積０のAABBoxを返す
		///※アニメーションによる変形は考慮されていない
		///※非同期読込完了までロックする
		AABBox GetMeshAABBox(const char* name);

		///ノード名を使って、メッシュごとのAABBoxを設定する
		///※現状では、GetAABBox()の結果の再計算は行っていない
		///※非同期読込完了までロックする
		void SetMeshAABBox(const char* name, const AABBox& aabb);
*/

#if defined __ENABLE_SUSPEND_RECOVER__
		virtual bool Restore();
		virtual bool Release();
#endif


	protected:
		FigureHeader* header;
		pyxieAnimator* animatorSlot[NumSlot];
		float	lastEvalTime[NumSlot];
		//pyxieShader** meshShaders;
		MeshInfo* meshinfo;
		FigureMaterial** materials;
		float blendWeight[3];
		uint32_t brendTreeType;
		void* blendTreeBranches;
		void* blendTreeLeaves;
		uint16_t numBlendBranches;
		uint16_t numBlendLeaves;

		//float* skinningMatrices;
		//float* inbindSkinningMatrices;
		//Transform*	localJointBuffer;

		float* userChannelAnimeBuffer;

		std::map<uint32_t, pyxieAnimator*> animes;

	private:
		void ClearMember();
		void CreateBlendTree();
		void CopyBlendWeight();
		bool BuildMesh();
		void DeleteMesh();
		void DrawMesh(FigureMesh& mesh, MeshInfo& info);
		bool LoopMesh(int& i);
	};
}