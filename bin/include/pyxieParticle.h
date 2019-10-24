#pragma once
#include "pyxieFigure.h"
#include <vector>
#include <map>

struct RenderSurface;

namespace pyxie
{
struct b2Vec2
{
    float x, y;
};

struct b2Color
{
    unsigned char r, g, b, a;
};



#define MAX_PARTICLES (1024 * 1024)

class PYXIE_EXPORT pyxieParticle : public pyxieFigure
{
	private:
		// gpu id		
		unsigned int m_vertexArrayID;
		unsigned int m_vertexArrayID_screen;

		unsigned int m_vertexBufferID;
		unsigned int m_positionBufferID;
		unsigned int m_positionBufferID_screen;
		
		unsigned int m_colorBufferID;
		unsigned int m_vertexArrayID_blurX;
		unsigned int m_vertexArrayID_blurY;
		unsigned int m_positionBufferID_blurX;
		unsigned int m_positionBufferID_blurY;
		unsigned int m_programID_blurX;
		unsigned int m_programID_blurY;

		unsigned int ublurTex_X;
		unsigned int ublurTex_Y;

		unsigned int ublurBufferSize_X;
		unsigned int ublurBufferSize_Y;

		// cpu data (shared with Box2D)
		b2Vec2* m_posBuff;
		b2Color* m_colorBuff;

		// gpu shader location
		unsigned int m_programID;
		unsigned int m_programID_screen;
		unsigned int m_cameraRightID;
		unsigned int m_cameraUpID;
		unsigned int m_particleSizeID;
		unsigned int m_viewProjMatrixID;
		unsigned int m_viewProjMatrixID_screen;
		//unsigned int m_textureSamplerID;

		// members
		int m_particleCount;
		float m_size;
		bool m_bInitialized;
		RenderSurface *m_RenderSurface;
		RenderSurface *m_BlurSurface;

		// camera
		pyxieCamera* m_camera;
		float m_ppm;

	public:
		pyxieParticle(b2Vec2* posBuff = NULL, b2Color* colorBuff = NULL, int count = 0, float size = -1);
		~pyxieParticle();

		void Init();
		void UpdateParticles(b2Vec2* posBuff = NULL, b2Color* colorBuff = NULL, int count = 0, float size = -1);
		virtual void Render();
		void RenderToScreen();
		void Blur();
		inline void SetPPM(float ppm) { m_ppm = ppm; }
		inline void SetCamera(pyxieCamera* cam) { m_camera = cam; }
		inline pyxieCamera* GetCamera(const char* name = nullptr) { return m_camera; }
		inline static pyxieParticle *new_particle(b2Vec2 *posBuff = NULL, b2Color *colorBuff = NULL, int count = 0, float size = -1)
		{
			return new pyxieParticle(posBuff, colorBuff, count, size);
		}

		void Build() {}
		void Initialize() {}
		void Clone(bool afterFinishBuild) {}
		RESOURCETYPE ResourceType() { return FIGURETYPE; }

		void Dump(const char* file) {}
		virtual void Pose() {}

		pyxieEnvironmentSet* GetEnvironmentSet() { return 0; }

		void BindAnimator(AnimatorSlot slot, pyxieAnimator* animator) {}
		void BindAnimator(AnimatorSlot slot, const char* name) {}
		inline pyxieAnimator* GetBindedAnimator(AnimatorSlot slot) { return 0; }
		inline void SetBlendingWeight(uint32_t ABC, float weight) {}
		inline float GetBlendingWeight(uint32_t ABC) { return 0.0f; }

		FigureHeader* GetHeader() { return 0; }
		int GetJointIndex(uint32_t jointHash) { return 0; }
		int GetJointParentIndex(uint32_t jointHash) { return 0; }
		int NumJoints() { return 0; }

		const Joint GetJoint(int idx) { return Joint(0); }
		const void SetJoint(int idx, const Joint& joint) {}
	};
} // namespace pyxie