#ifndef _CHROMA_MODELLOADER_H_
#define _CHROMA_MODELLOADER_H_

#include <common/CoreCommon.h>
// thirdparty
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// chroma
#include <math/Math.h>
#include <model/Model.h>

namespace Chroma {
	class ModelLoader
	{
		static void GetChildMeshNodes(aiNode* node,const aiScene*& scene, std::vector<MeshComponent*>& meshList);
		static MeshComponent* ProcessMesh(aiMesh* mesh, const aiScene* scene);
		// skinning and skeletons
		static void SetVertSkinningData(ChromaSkinnedVertex& vert, std::pair<int, float>  const& jointIDWeight);
		static void ProcessSkeleton(const aiScene* scene, const aiMesh* mesh, Skeleton& skeleton);
		static void GetChildJointIDs(aiNode* node, Skeleton& skeleton, std::vector<int>& childJointIDs);
		static void GetParentJointID(const aiNode* node, Skeleton& skeleton, int& parentJointID);
		static void NormalizeSkinningWeights();
		// textures
		static std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, Texture::TYPE typeName);

	public:
		static std::vector<MeshComponent*> Load(std::string const& sourcePath);

	};
}

#endif