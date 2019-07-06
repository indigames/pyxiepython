#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>

class FCDGeometryInstance;
class FCDGeometryPolygons;
class FCDMaterial;
class FCDGeometry;
class FCDSceneNode;
class FCDMaterial;
class FCDocument;
class FCDLight;
class FCDPhysicsRigidBodyInstance;
class FCDEmitterInstance;
class FCDEmitter;
class FCDAsset;
class FCDSkinController;
class FCDEffectParameter;

namespace pyxie
{

	// helper struct for collecting all the polygonal data in the scene
	struct PolygonsInstance
	{
		const FCDGeometryInstance*	m_geometryInstance;
		const FCDGeometryPolygons*	m_polygons;
		const FCDMaterial*			m_material;
	};

	class pyxieColladaCollection
	{
		std::string filename;
	public:
		pyxieColladaCollection();
		~pyxieColladaCollection();
		bool OpenCollada(const char* inputFile);
		void CollectGeometry();
		//void WriteNameList(const char* path);

		FCDocument *document;
		std::vector<const FCDSceneNode*> joints;
		std::vector< PolygonsInstance > instances;
		std::set<const FCDGeometry*> blendShapeTargets;
		std::vector<const FCDSceneNode*> xrefs;
		std::map< const FCDMaterial*, uint32_t> materials;
		std::vector<FCDLight*> lights;
		std::vector<const FCDPhysicsRigidBodyInstance*> rigidBodies;
		//std::vector< EmittersInstance> emitters;
		FCDAsset* asset;

	private:
		void	FindBlendShapeTargets(const FCDocument* document, std::set<const FCDGeometry*>& blendShapeTargets);
		void	FindXRefNodes(const FCDSceneNode* pNode, std::vector<const FCDSceneNode*>& nodes);
		void	GetPolygons(const FCDSceneNode* sceneNode,std::vector< PolygonsInstance >& instances,const std::set<const FCDGeometry*>& blendShapeTargets);
		void	GetPolygons(const FCDocument* document,std::vector< PolygonsInstance >& instances,const std::set<const FCDGeometry*>& blendShapeTargets);

		//void ColladaCollection::GetEmitters( const FCDocument* document, std::vector< EmittersInstance >& instances);
		//void ColladaCollection::GetEmitters( const FCDSceneNode* sceneNode, std::vector< EmittersInstance >& instances );
		//std::string MakeLightName(std::string name, int type, int& no);
	};
}