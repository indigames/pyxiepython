///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////
#pragma once
#include "pyxieResource.h"
#include "pyxieMathutil.h"
#include "pyxieRenderContext.h"

namespace pyxie
{
		class PYXIE_EXPORT pyxieDrawable : public pyxieResource
		{
		protected:
			Transform rootTransform;
			const float* parentJoint;
		public:
			pyxieDrawable() : parentJoint(nullptr) {
				rootTransform.translation = Vec3();
				rootTransform.scale = Vec4(1.0f,1.0f,1.0f,1.0f);
				rootTransform.rotation =Quat();
			}
			pyxieDrawable(const pyxieDrawable* org) : pyxieResource(org) , parentJoint(nullptr) {
				rootTransform.translation = Vec3();
				rootTransform.scale = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
				rootTransform.rotation = Quat();
			}

			virtual void Pose() {}
			virtual void Render() {}

			inline void SetPosition(const Vec3& pos) { rootTransform.translation = pos; }
			inline const Vec3& GetPosition() const { return rootTransform.translation; }
			inline void SetRotation(const Quat& rot) { rootTransform.rotation = rot; }
			inline const Quat& GetRotation() const { return rootTransform.rotation; }
			inline void SetScale(const Vec3& scale) { rootTransform.scale = (const Vec4&)scale; rootTransform.scale.W(1.0f); }
			inline const Vec3& GetScale() { return (const Vec3&)rootTransform.scale; }
		protected:
			inline bool GetParentJointMatrix(Mat4& outMatrix) const
			{
				if (!parentJoint) return false;
				JointToMatrix4(parentJoint, outMatrix);
				return true;
			}

			void processBlendTree(float* outputMatrices,
				const void* ppuContext,
				uint32_t spuId,
				const void* skeleton,
				const void* blendBranches,
				uint32_t numBlendBranches,
				const void* blendLeaves,
				uint32_t numBlendLeaves,
				const Transform* rootJoint,
				void* scratchBuffer,
				uint32_t sizeScratchBuffer,
				float* outputUserChannelAnime,
				Transform* localPoses);
		};
}
