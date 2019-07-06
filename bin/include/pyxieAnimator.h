///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////
#pragma once
#include "pyxieResource.h"

namespace pyxie
{

	class PYXIE_EXPORT pyxieAnimator : public pyxieResource
	{
		bool isLoop;
		bool outSource;
		float totalEvalTime;
		float evalTime;
		float elapsedTime;
		float startTime;
		float endTime;
		float speed;

		void* animationSet;
	public:
		pyxieAnimator(const char* path, void* set=nullptr);
		pyxieAnimator(pyxieAnimator* org);
		~pyxieAnimator();
		void Build();
		void Initialize();
		void Clone(bool afterFinishBuild);
		RESOURCETYPE ResourceType() { return ANIMATORTYPE; }

		void* GetAnimationSet(){
			WaitInitialize();
			return animationSet;
		}

		inline void SetLoop(bool enable){isLoop = enable;}
		inline bool IsLoop() { return isLoop; }

		//現在のアニメーションフレーム
		inline float GetEvalTime() { return evalTime; }
		inline void SetEvalTime(float time) { evalTime = time; }

		//累積のアニメーションフレーム
		inline float GetTotalEvalTime() { return totalEvalTime; }
		inline void SetTotalEvalTime(float time) { totalEvalTime = time; }

		///アニメーションスタートタイム
		inline float GetStartTime(){return startTime;}
		inline void SetStartTime(float time) { startTime = time; }

		///アニメーション終了時間（秒）
		inline void SetEndTime(float time){endTime = time;}
		inline float GetEndTime(){
			if (endTime != 0.0f) return endTime;
			WaitInitialize();
			return endTime;
		}

		///アニメーション再生速度
		inline void SetSpeed(float s){speed = s;}
		inline float GetSpeed(){return speed;}

		///初期状態のアニメーション終了時間（秒）を取得する
		float GetDefaultEndtime();

		//現在の時間をアニメーションスタートタイムに戻す
		inline void Rewind() { evalTime = startTime; }

		//経過時間
		inline float GetElapsedTime() { return elapsedTime; }

		//時間を進める（マイナスなら逆再生）
		void Step(float elapsedTime);

	private:
		void ClearMember();
	};
}