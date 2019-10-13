///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////
#pragma once

#include "pyxieResource.h"
#include "pyxieMathutil.h"
#include "pyxieFigurestruct.h"

namespace pyxie
{


	class pyxieFigure;

	class PYXIE_EXPORT pyxieEnvironmentSet : public pyxieResource {

		pyxieFigure* parent;
		EnvironmentSource* ambient;
		EnvironmentSource* dirLamp[3];
		EnvironmentSource* pointLamp[7];
		EnvironmentSource* distfog;

		enum {
			new_ambient=0,
			new_dir0,
			new_dir1,
			new_dir2,
			new_point0,
			new_point1,
			new_point2,
			new_point3,
			new_point4,
			new_point5,
			new_point6,
			new_fog,
			num_new
		};
		uint32_t newflag;

	public:
		pyxieEnvironmentSet(pyxieFigure* figure);
		pyxieEnvironmentSet(const char* name);
		pyxieEnvironmentSet(pyxieEnvironmentSet* org);
		~pyxieEnvironmentSet();
		void Build();
		void Initialize();
		void Clone(bool afterFinishBuild);
		RESOURCETYPE ResourceType() { return ENVIRONMENTSETTYPE; }
		void Render();
		const Vec3 GetAmbientSkyColor();
		void SetAmbientSkyColor(const Vec3& col);
		const Vec3 GetAmbientGroundColor();
		void SetAmbientGroundColor(const Vec3& col);
		Vec3 GetAmbientDirection();
		void SetAmbientDirection(const Vec3& dir);
		float GetDirectionalLampIntensity(uint32_t index);
		void SetDirectionalLampIntensity(uint32_t index, float intensity);
		const Vec3 GetDirectionalLampColor(uint32_t index);
		void SetDirectionalLampColor(uint32_t index, const Vec3& col);
		Vec3 GetDirectionalLampDirection(uint32_t index);
		void SetDirectionalLampDirection(uint32_t index, const Vec3& dir);
		float GetPointLampRange(uint32_t index);
		void SetPointLampRange(uint32_t index, float range);
		float GetPointLampIntensity(uint32_t index);
		void SetPointLampIntensity(uint32_t index, float intensity);
		const Vec3 GetPointLampColor(uint32_t index);
		void SetPointLampColor(uint32_t index, const Vec3& col);
		Vec3 GetPointLampPosition(uint32_t index);
		void SetPointLampPosition(uint32_t index, const Vec3& pos);
		float GetDistanceFogNear();
		void SetDistanceFogNear(float _near);
		float GetDistanceFogFar();
		void SetDistanceFogFar(float _far);
		float GetDistanceFogAlpha();
		void SetDistanceFogAlpha(float alpha);
		const Vec3 GetDistanceFogColor();
		void SetDistanceFogColor(const Vec3& col);
	public:
		EnvironmentSource* CreateAmbient();
		EnvironmentSource* CreateDir(int idx);
		EnvironmentSource* CreatePoint(int idx);
		EnvironmentSource* CreateFog();
	};
}