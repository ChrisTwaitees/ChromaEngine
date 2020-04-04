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
#include <resources/TextureLoader.h>
#include <jobSystem/JobSystem.h>

namespace Chroma {
	class ModelLoader
	{
	public:
		static std::vector<MeshData> Load(std::string const& sourcePath);
		static void LoadThreadSafe(std::string const& sourcePath, std::vector<MeshData>& meshList);

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
		static void GetChildJointIDs(const aiNode* node, Skeleton& skeleton, std::vector<int>& childJointIDs);
		static void GetParentJointID(const aiNode* node, Skeleton& skeleton, int& parentJointID);
		static void NormalizeSkinningWeights(MeshData& meshData);
		// textures
		static void GetTexturesFromMaterial(aiMaterial* mat, aiTextureType type, Texture::TYPE typeName, MeshData& meshData);
	};
}

#endif