#pragma once

#include "pyxieEditableFigure.h"
#include "pyxieTypes.h"
#include "pyxieColladaCollection.h"

namespace pyxie
{
	typedef std::vector<uint32_t> UniqueVert;

	struct	UniqueVertCompare {
		bool operator()(const UniqueVert& c1, const UniqueVert& c2) const {
			uint32_t size = (uint32_t)c1.size();
			for (uint32_t i = 0; i < size; ++i) {
				uint32_t v1 = c1[i];
				uint32_t v2 = c2[i];
				if (v1 < v2)
					return true;
				if (v1 > v2)
					return false;
			}
			return false;
		}
	};

	class PYXIE_EXPORT pyxieColladaLoader : public pyxieObject
	{
		//std::vector<TextureSource> textureSources;
		//std::vector<std::string> jointNames;

	public:
		pyxieColladaLoader();
		~pyxieColladaLoader();

		bool LoadCollada(const char* path, pyxieEditableFigure* editableFigure);
		bool LoadColladaAnimation(const char* path, pyxieEditableFigure* editableFigure);

	private:
		void LoadLightAndCamera(const std::vector<const FCDSceneNode*>& joints, const std::vector<FCDLight*>& lights, pyxieEditableFigure* editableFigure);
		void GetInvBindMatrices(std::vector< PolygonsInstance >& instances, std::vector<const FCDSceneNode*>& joints, pyxieEditableFigure* editableFigure);
		void ConvertTryStrip(pyxieEditableFigure* editableFigure);
		void GetScenes(std::vector< PolygonsInstance >& instances, std::map< const FCDMaterial*, uint32_t >& materials, std::vector<const FCDSceneNode*> joints, pyxieEditableFigure* editableFigure);
		bool CreateScene(const PolygonsInstance& instance, const std::vector<const FCDSceneNode*>& joints, EditableMesh *emesh, uint32_t processingFlags, pyxieEditableFigure* editableFigure);
		void GetVertexesAndPolygons(const FCDGeometryPolygons *polys, std::map<UniqueVert, uint32_t, UniqueVertCompare> *outUniqueVerts, std::vector<int32_t> *outPolyList, pyxieEditableFigure* editableFigure);
		void CreateSkinData(const FCDSceneNode* jointNode, const std::vector<const FCDSceneNode*>& joints, EditableMesh* emesh, pyxieEditableFigure* editableFigure);
		void CopySkinData(const FCDSkinController* skin, const std::vector<const FCDSceneNode*>& joints, const std::map<UniqueVert, uint32_t, UniqueVertCompare>& uniqueVerts, int32_t positionInputIndex, EditableMesh *emesh, pyxieEditableFigure* editableFigure);
		void LoadMaterials(std::map< const FCDMaterial*, uint32_t >& materials, pyxieEditableFigure* editableFigure);
		bool GetMaterialParam(const FCDEffectParameter* fparam, FigureMaterialParam& oparam, std::map<std::string, TextureSource >& textures, pyxieEditableFigure* editableFigure);
	};

}
