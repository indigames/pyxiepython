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
	public:
		enum Mode
		{
			Mode_FlyThru,	// Camera moves through the world
			Mode_LookAt,	// Camera looks at a single point
		};

	private:
		pyxieFigure*	parent;
		FigureCamera*	figureCamera;

		Mode		mMode;				// Camera mode
		float		mPan;				// Mode_FlyThruモードの時のＹ軸回転
		float		mTilt;				// Mode_FlyThruモードの時のＸ軸回転
		float		mDist;				// Mode_LookAtモードの時のターゲットからの距離
		Vec3		mTarget;			// Mode_LookAtモードのターゲット座標

		Vec2		screenScale;
		Vec2		screenOffset;
		float		screenRadian;

	public:
		pyxieCamera(const char* name);
		pyxieCamera(pyxieFigure* figure, FigureCamera* figureCam);
		pyxieCamera(pyxieCamera* org);
		~pyxieCamera();

		void Clone(bool afterFinishBuild);
		void Build() {}
		void Initialize() {}
		RESOURCETYPE ResourceType() { return CAMERATYPE; }

		//視野角
		inline float GetFieldOfView() const { return figureCamera->fov; }
		inline void SetFieldOfView(float val) { figureCamera->fov = val; }

		//アスペクト比
		float GetAspectRate();
		void SetAspectRate(float val);

		//平行投影カメラのサイズ
		//VirtualScreenWidth/2のサイズでフルスクリーンカメラになる
		inline float GetOrthoWidth() const { return figureCamera->magX; }
		inline void SetOrthoWidth(float val) { figureCamera->magX = val; }

		//ニアクリッププレーン
		inline float GetNearPlane() const { return figureCamera->nearclip; }
		inline void SetNearPlane(float val) { figureCamera->nearclip = val; }

		//ファークリッププレーン
		inline float GetFarPlane() const { return figureCamera->farclip; }
		inline void SetFarPlane(float val) { figureCamera->farclip = val; }

		//カメラモード
		inline Mode GetMode() const { return mMode; }
		inline void SetMode(Mode mode) { mMode = mode; }

		//Ｙ軸回転
		void SetPan(float pan);
		float GetPan() const;

		//Ｘ軸回転
		void SetTilt(float tilt);
		float GetTilt() const;

		//ターゲットからの距離(Mode_LookAtモード用)
		inline void SetDistance(float dist) { mDist = dist; }
		inline float GetDistance() const { return mDist; }

		//ターゲット座標(Mode_LookAtモード用)
		inline void SetTarget(const Vec3 &tar) { mTarget = tar; }
		inline Vec3 GetTarget() const { return mTarget; }

		/**@brief ターゲットを中心にしてカメラを回転する(Mode_LookAtモード用)
		 * @param speed::x x軸周りの回転速度(単位radian)
		 * @param speed::y y軸周りの回転速度
		 * @param speed::z z軸周りの回転速度  */
		void RotateAroundTarget(const Vec3 &speed);

		//平行投影カメラ
		inline void SetOrthographicProjection(bool enable) { figureCamera->isOrtho = (uint32_t)enable; }
		inline bool IsOrthographicProjection() { return (bool)figureCamera->isOrtho; }

		void	Step(float elapsedTime);

		//カメラを有効にする
		void	Render();

		//影用カメラ
		void	RenderShadowView(pyxieCamera* viewCam);

		//プロジェクション行列を計算して返す
		Mat4& GetProjectionMatrix(Mat4& out);

		//逆ビュー行列を計算して返す（ビュー行列が必要ならinverse()で）
		Mat4& GetViewInverseMatrix(Mat4& out) const;

		//スクリーンマトリックスを計算して返す
		Mat4& GetScreenMatrix(Mat4& out);

		void	SetScreenScale(Vec2& v) { screenScale = v; }
		void	SetScreenOffset(Vec2& v) { screenOffset = v; }
		void	SetScreenRadian(float r) { screenRadian = r; }
		const Vec2&	GetScreenScale() const { return screenScale; }
		const Vec2&	GetScreenOffset() const { return screenOffset; }
		float	GetScreenRadian() const { return screenRadian; }
	};

}