///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////
#pragma once

#include "pyxieTypes.h"
namespace pyxie
{
	struct SampleShapeVertex
	{
		float pos[4];
		float norm[3];
		float uv[2];
		float color[4];
	};
	void MakeSprite(SampleShapeVertex* pVertices, uint32_t* pPointRep, uint16_t* pwIndices, float fWidth, float fHeight, float fDepth, float fOfsX, float fOfsY, float fOfsZ);
	void MakeBox(SampleShapeVertex* pVertices, uint32_t* pPointRep, uint16_t* pwIndices, float fWidth, float fHeight, float fDepth);
	void MakeCylinder(SampleShapeVertex* pVertices, uint32_t* pPointReps, uint16_t* pwIndices, float fRadius1, float fRadius2, float fLength, uint32_t uSlices, uint32_t uStacks);
	void MakePolygon(SampleShapeVertex* pVertices, uint16_t* pwIndices, uint32_t numPoints, float* points, float* uvs, float* normals, float* colors, uint32_t numIndices, uint16_t* indices, uint32_t maxVerticies, uint32_t maxIndices);
	void MakeSphere(SampleShapeVertex* pVertices, uint16_t* pwIndices, float fRadius, uint32_t uSlices, uint32_t uStacks);
	void MakeTorus(SampleShapeVertex* pVertices, uint16_t* pwIndices, float fInnerRadius, float fOuterRadius, uint32_t uSides, uint32_t uRings);
	void MakeTeapot(SampleShapeVertex* pVertices, uint16_t* pwIndices, float fSize);
	uint32_t NumFacesSprite();
	uint32_t NumVerticesSprite();
	uint32_t NumFacesBox();
	uint32_t NumVerticesBox();
	uint32_t NumFacesCylinder(uint32_t uStacks, uint32_t uSlices);
	uint32_t NumVerticesCylinder(uint32_t uStacks, uint32_t uSlices);
	uint32_t NumFacesSphere(uint32_t uStacks, uint32_t uSlices);
	uint32_t NumVerticesSphere(uint32_t uStacks, uint32_t uSlices);
	uint32_t NumFacesTorus(uint32_t uSides, uint32_t uRings);
	uint32_t NumVerticesTorus(uint32_t uSides, uint32_t uRings);
	uint32_t NumFacesTeapot();
	uint32_t NumVerticesTeapot();
}


