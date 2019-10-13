///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////
#pragma once

#include "pyxieUtilities.h"
#include "pyxieDrawable.h"
#include "pyxieFigurestruct.h"

namespace pyxie
{
	class pyxieFigure;

	class PYXIE_EXPORT pyxieCamera : public pyxieDrawable
	{
	private:
		pyxieFigure*	parent;
		FigureCamera*	figureCamera;

		bool		mLockon;
		float		mPan;
		float		mTilt;
		float		mRoll;
		Vec3		mTarget;

		Vec2		screenScale;
		Vec2		screenOffset;
		float		screenRadian;
		int			upaxis;
	public:
		pyxieCamera(const char* name);
		pyxieCamera(pyxieFigure* figure, FigureCamera* figureCam);
		pyxieCamera(pyxieCamera* org);
		~pyxieCamera();

		void Clone(bool afterFinishBuild);
		void Build() {}
		void Initialize() {}
		RESOURCETYPE ResourceType() { return CAMERATYPE; }
		void Step(float elapsedTime);
		void Render();

		void SetCameraPosition(const Vec3& pos);
		void SetCameraRotation(const Quat& rot);


		//視野角
		inline float GetFieldOfView() const { return figureCamera->fov; }
		inline void SetFieldOfView(float val) { figureCamera->fov = val; }

		//アスペクト比
		float GetAspectRate();
		void SetAspectRate(float val);

		//平行投影カメラのサイズ
		inline float GetOrthoWidth() const { return figureCamera->magX; }
		inline void SetOrthoWidth(float val) { figureCamera->magX = val; }

		//ニアクリッププレーン
		inline float GetNearPlane() const { return figureCamera->nearclip; }
		inline void SetNearPlane(float val) { figureCamera->nearclip = val; }

		//ファークリッププレーン
		inline float GetFarPlane() const { return figureCamera->farclip; }
		inline void SetFarPlane(float val) { figureCamera->farclip = val; }

		//Y軸回転
		void SetPan(float pan);
		float GetPan() const;

		//X軸回転
		void SetTilt(float tilt);
		float GetTilt() const;

		//Z軸回転
		void SetRoll(float roll);
		float GetRoll() const;

		void SetTarget(const Vec3& tar);
		Vec3 GetTarget() const;
		void LockonTarget(bool lockon = true);

		inline bool GetLockon() {return mLockon;}

		//平行投影カメラ
		inline void SetOrthographicProjection(bool enable) { figureCamera->isOrtho = (uint32_t)enable; }
		inline bool IsOrthographicProjection() { return (bool)figureCamera->isOrtho; }

		//影用カメラ
		void RenderShadowView(pyxieCamera* viewCam);

		//プロジェクション行列を計算して返す
		Mat4& GetProjectionMatrix(Mat4& out);

		//逆ビュー行列を計算して返す（ビュー行列が必要ならinverse()で）
		Mat4& GetViewInverseMatrix(Mat4& out) const;

		//スクリーンマトリックスを計算して返す
		Mat4& GetScreenMatrix(Mat4& out);

		void SetScreenScale(Vec2& v) { screenScale = v; }
		void SetScreenOffset(Vec2& v) { screenOffset = v; }
		void SetScreenRadian(float r) { screenRadian = r; }
		const Vec2&	GetScreenScale() const { return screenScale; }
		const Vec2&	GetScreenOffset() const { return screenOffset; }
		float GetScreenRadian() const { return screenRadian; }

		void SetUpAxis(int ax) { upaxis = ax; }

	private:
		void update();
	};

}