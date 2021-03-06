#ifndef _CHROMA_MODELLOADER_H_
#define _CHROMA_MODELLOADER_H_

#include <common/CoreCommon.h>
// thirdparty
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// chroma
#include <math/Math.h>
#include <model/MeshData.h>
#include <jobSystem/JobSystem.h>
#include <animation/Skeleton.h>
#include <serialization/formats/Json.h>

namespace Chroma {
	class ModelLoader
	{
	public:
		// Init
		static void Init();

		static std::vector<MeshData> Load(std::string const& sourcePath);
		static void LoadThreadSafe(std::string const& sourcePath, std::vector<MeshData>& meshList);
		static void CalculateBBoxCentroid(MeshData& meshData);

	private:
		// sourceDir of last import
		static std::string m_SourceDir;
		// mesh
		static MeshData ProcessMesh(aiMesh* mesh, const aiScene*& scene);
		static void GetChildMeshNodes(aiNode* node,const aiScene*& scene, std::vector<MeshData>& meshDatas);
		// skinning and skeletons
		static void SetVertSkinningData(ChromaSkinnedVertex& vert, std::pair<int, float>  const& jointIDWeight);
		static void ProcessSkeleton(const aiScene* scene, const aiMesh* mesh, Skeleton& skeleton, MeshData& meshData);
		static void ProcessSkeletonMetaData(const aiScene* scene, const aiMesh* mesh, Skeleton& skeleton, MeshData& meshData);
		static void ProcessSkeletonConstraints(const aiMetadata* metaData, unsigned int propertyIndex, Skeleton& skeleton);
		static void GetChildJointIDs(const aiNode* node, Skeleton& skeleton, std::vector<int>& childJointIDs);
		static void GetParentJointID(const aiNode* node, Skeleton& skeleton, int& parentJointID);
		static void NormalizeSkinningWeights(MeshData& meshData);
		static std::vector<IKConstraint> GetIKConstraints(JSON& metaData, Skeleton const& skeleton);
		// textures
		static void GetTexturesFromMaterial(aiMaterial* mat, aiTextureType type, Chroma::Type::Texture typeName, MeshData& meshData);
		// clean up
	};
}



// MD - MetaData

// Skeleton Constraints
#define CONSTRAINT_MD_ROOTKEY "CHROMA_CONSTRAINT"
#define CONSTRAINT_MD_NAME_KEY "name"
#define CONSTRAINT_MD_IK_KEY "IK"
#define CONSTRAINT_MD_IK_ROOT_KEY "root"
#define CONSTRAINT_MD_IK_EFFECTOR_KEY "effector"
#define CONSTRAINT_MD_AIM_KEY "AIM"
#define CONSTRAINT_MD_PARENT_KEY "PARENT"
#define CONSTRAINT_MD_ORIENT_KEY "ORIENT"

#endif