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
			enum
			{
				state_updateTransform = (state_resource_last << 1),
				state_posed = (state_updateTransform << 1),
				state_drawable_last = state_posed
			};
			Joint rootTransform;
			const float* parentJoint;
		public:
			pyxieDrawable() : parentJoint(nullptr) {
			}
			pyxieDrawable(const pyxieDrawable* org) : pyxieResource(org) , parentJoint(nullptr) {
			}

			virtual void Pose() {}
			virtual void Render() {}

			inline void SetPosition(const Vec3& pos)
			{ 
				if (!vmath_almostEqual(rootTransform.translation.P(), pos.P(), 3)) {
					rootTransform.translation = pos;
					SetState(state_updateTransform);
					ResetState(state_posed);
				}
			}
			inline const Vec3& GetPosition() const {
				return rootTransform.translation;
			}
			inline void SetRotation(const Quat& rot){ 
				if (!vmath_almostEqual(rootTransform.rotation.P(), rot.P(), 4)) {
					rootTransform.rotation = rot;
					SetState(state_updateTransform);
					ResetState(state_posed);
				}
			}
			inline const Quat& GetRotation() const {
				return rootTransform.rotation; 
			}
			inline void SetScale(const Vec3& scale) {
				if (!vmath_almostEqual(rootTransform.scale.P(), scale.P(), 3)) {
					rootTransform.scale = (const Vec4&)scale; rootTransform.scale.W(1.0f);
					SetState(state_updateTransform);
					ResetState(state_posed);
				}
			}
			inline const Vec3& GetScale() { return (const Vec3&)rootTransform.scale; }
		protected:
			inline bool GetParentJointMatrix(Mat4& outMatrix) const{
				if (!parentJoint) return false;
				JointMatrixToMatrix4(parentJoint, outMatrix);
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
				const Joint* rootJoint,
				void* scratchBuffer,
				uint32_t sizeScratchBuffer,
				float* outputUserChannelAnime,
				Joint* localPoses);
		};
}
