#pragma once
#include "pyxieFigure.h"
#include <vector>
#include <map>

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
    unsigned int m_textureID;      // texture
    unsigned int m_vertexArrayID;  //vao
    unsigned int m_vertexBufferID; //vbo
    unsigned int m_positionBufferID;
    unsigned int m_colorBufferID;

    // cpu data (shared with Box2D)
    b2Vec2 *m_posBuff;
    b2Color *m_colorBuff;

    // gpu shader location
    unsigned int m_programID;
    unsigned int m_cameraRightID;
    unsigned int m_cameraUpID;
    unsigned int m_particleSizeID;
    unsigned int m_viewProjMatrixID;
    unsigned int m_textureSamplerID;

    // members
    int m_particleCount;
    float m_size;

public:
    pyxieParticle(b2Vec2 *posBuff = NULL, b2Color *colorBuff = NULL, int count = 0, float size = -1);
    ~pyxieParticle();

    static pyxieParticle *new_particle(b2Vec2 *posBuff = NULL, b2Color *colorBuff = NULL, int count = 0, float size = -1);

    virtual void Render();
    void Init();
    void DestroyParticle(int idx);

    void Build() {}
    void Initialize() {}
    void Clone(bool afterFinishBuild) {}
    RESOURCETYPE ResourceType() { return FIGURETYPE; }

    void Dump(const char *file) {}
    virtual void Pose() {}

    pyxieEnvironmentSet *GetEnvironmentSet() { return 0; }
    pyxieCamera *GetCamera(const char *name = nullptr) { return 0; }

    void BindAnimator(AnimatorSlot slot, pyxieAnimator *animator) {}
    void BindAnimator(AnimatorSlot slot, const char *name) {}
    inline pyxieAnimator *GetBindedAnimator(AnimatorSlot slot) { return 0; }
    inline void SetBlendingWeight(uint32_t ABC, float weight) {}
    inline float GetBlendingWeight(uint32_t ABC) { return 0.0f; }

    FigureHeader *GetHeader() { return 0; }
    int GetJointIndex(uint32_t jointHash) { return 0; }
    int GetJointParentIndex(uint32_t jointHash) { return 0; }
    int NumJoints() { return 0; }

    const Joint GetJoint(int idx) { return Joint(0); }
    const void SetJoint(int idx, const Joint &joint) {}
};
} // namespace pyxie