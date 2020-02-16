#include "ModelLoader.h"

namespace Chroma 
{
	std::string ModelLoader::m_SourceDir;

	// LOADING
	std::vector<MeshData> ModelLoader::Load(std::string const& sourcePath)
	{
		// Debug
		CHROMA_TRACE_UNDERLINE;
		CHROMA_TRACE("MODEL LOADER :: Loading MeshData from: {}", sourcePath);

		// set source directory
		m_SourceDir = sourcePath.substr(0, sourcePath.find_last_of('/'));

		// vector of collected mesh datas
		std::vector<MeshData> meshDatas;
		
		// assimp importer
		Assimp::Importer importer;
		//const aiScene* scene = importer.ReadFile(sourcePath, aiProcess_Triangulate | aiProcess_FlipUVs |aiProcess_GenSmoothNormals  | aiProcess_CalcTangentSpace);
		const aiScene* scene = importer.ReadFile(sourcePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			CHROMA_WARN("ERROR::ASSIMP:: {0}", importer.GetErrorString());
			return meshDatas;
		}

		// iterate through the scenes meshes
		GetChildMeshNodes(scene->mRootNode, scene, meshDatas);
		// set the source path on all discovered mesh datas
		for(MeshData& meshData : meshDatas)
			meshData.sourceDirectory = m_SourceDir;

		// Debug
		CHROMA_TRACE("MODEL LOADER :: MeshData Loaded");
		CHROMA_TRACE_UNDERLINE;

		// return 
		return meshDatas;
	}

	void ModelLoader::GetChildMeshNodes(aiNode* node, const aiScene*& scene, std::vector<MeshData>& meshList)
	{
		// process node's meshes (if it has any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshList.push_back(ProcessMesh(mesh, scene));
		}
		// check if node has children, if so recursively search for meshes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			GetChildMeshNodes(node->mChildren[i], scene, meshList);
		}
	}

	MeshData ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene*& scene)
	{
		MeshData newMeshData;

		// check if mesh is skinned
		newMeshData.isSkinned = mesh->HasBones();

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			// process vertex
			// positions
			glm::vec3 position;
			position.x = mesh->mVertices[i].x;
			position.y = mesh->mVertices[i].y;
			position.z = mesh->mVertices[i].z;

			// normals
			glm::vec3 normal;
			normal.x = mesh->mNormals[i].x;
			normal.y = mesh->mNormals[i].y;
			normal.z = mesh->mNormals[i].z;

			// tangents
			glm::vec3 tangent;
			if ( mesh->mTangents != NULL)
			{
				tangent.x = mesh->mTangents[i].x;
				tangent.y = mesh->mTangents[i].y;
				tangent.z = mesh->mTangents[i].z;
			}

			// bitangents 
			glm::vec3 bitangent;
			if ( mesh->mBitangents != NULL)
			{
				bitangent.x = mesh->mBitangents[i].x;
				bitangent.y = mesh->mBitangents[i].y;
				bitangent.z = mesh->mBitangents[i].z;
			}

			// texture coords
			glm::vec2 UV1;
			if (mesh->mTextureCoords[0])
			{
				UV1.x = mesh->mTextureCoords[0][i].x;
				UV1.y = mesh->mTextureCoords[0][i].y;
			}
			else
				UV1 = glm::vec2(0.0f, 0.0f);

			// add new vertex
			if (newMeshData.isSkinned)
			{
				ChromaSkinnedVertex vertex;
				vertex.m_position = position;
				vertex.m_normal = normal;
				vertex.m_tangent = tangent;
				vertex.m_bitangent = bitangent;
				vertex.m_texCoords = UV1;
				newMeshData.skinnedVerts.push_back(vertex);
			}
			else
			{
				ChromaVertex vertex;
				vertex.m_position = position;
				vertex.m_normal = normal;
				vertex.m_tangent = tangent;
				vertex.m_bitangent = bitangent;
				vertex.m_texCoords = UV1;
				newMeshData.verts.push_back(vertex);
			}
		}

		// process indices (faces)
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				newMeshData.indices.push_back(face.mIndices[j]);
		}

		// process material, fetching textures
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			// diffuse textures
			GetTexturesFromMaterial(material, aiTextureType_DIFFUSE, Texture::ALBEDO, newMeshData);
			// metalness textures
			GetTexturesFromMaterial(material, aiTextureType_SPECULAR, Texture::METALNESS, newMeshData);
			// normal textures
			GetTexturesFromMaterial(material,	aiTextureType_HEIGHT, Texture::NORMAL, newMeshData);
		}

		// build Mesh
		if (newMeshData.isSkinned)
		{
			// Process Skeleton
			Skeleton skeleton;			
			// Process Joint Hierarchy
			ProcessSkeleton(scene, mesh, skeleton, newMeshData);
			// Set MeshData's Skeleton
			newMeshData.skeleton = skeleton;
		}

		return newMeshData;
	}

	void ModelLoader::GetTexturesFromMaterial(aiMaterial * mat, aiTextureType type, Texture::TYPE typeName, MeshData& meshData)
	{
		// iterate through meshes textures
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString textureName;
			mat->GetTexture(type, i, &textureName);
			bool skip{ false };
			for (unsigned int j = 0; j < meshData.textures.size(); j++)
			{
				if (std::strcmp(meshData.textures[j].path.data(), textureName.C_Str()) == 0)
				{
					meshData.textures.push_back(meshData.textures[j]);
					skip = true;
					break;
				}
			}
			if (!skip)
			{
				Texture texture;
				texture.loadFromFile(textureName.C_Str(), m_SourceDir);
				texture.type = typeName;
				meshData.textures.push_back(texture);
				meshData.textures.push_back(texture);
			}
		}
	}

	void ModelLoader::SetVertSkinningData(ChromaSkinnedVertex & vert, std::pair<int, float> const& jointIDWeight)
	{
		for (int i = 0; i < MAX_VERT_INFLUENCES; i++)
		{
			if (vert.m_jointWeights[i] < jointIDWeight.second)
			{
				vert.m_jointIDs[i] = jointIDWeight.first;
				vert.m_jointWeights[i] = jointIDWeight.second;
				return;
			}
		}
	}

	void ModelLoader::ProcessSkeleton(const aiScene * scene, const aiMesh * mesh, Skeleton & skeleton, MeshData& meshData)
	{
		// Build Skeleton 
		for (unsigned int i = 0; i < mesh->mNumBones; i++)
		{
			// fetch assimp bone, copy data to chroma joint
			aiBone* bone = mesh->mBones[i];
			Joint newJoint;
			// Joint Name
			newJoint.m_Name = bone->mName.C_Str();
			// Joint Model Transform, Relative to Model Origin
			newJoint.m_ModelBindTransform = glm::inverse(AIToGLM(bone->mOffsetMatrix));
			// Joint Final Transform intialized to model transform
			newJoint.m_ModelSpaceTransform = glm::inverse(AIToGLM(bone->mOffsetMatrix));
			// Joint Inverse Model Transform, Relative from Joint to Origin
			newJoint.m_ModelInverseBindTransform = AIToGLM(bone->mOffsetMatrix);
			// Joint ID
			newJoint.m_ID = i;

			// store joint IDs and Weights to skelton and verts
			for (unsigned int j = 0; j < bone->mNumWeights; j++)
			{
				// update joint
				aiVertexWeight vertexWeight = bone->mWeights[j];
				// Joint ID, Weight
				std::pair<unsigned int, float> skinningData;
				skinningData.first = i;
				skinningData.second = vertexWeight.mWeight;
				SetVertSkinningData(meshData.skinnedVerts[vertexWeight.mVertexId], skinningData);
			}
			// Add new joint
			skeleton.AddJoint(newJoint);
		}
		// Normalize Skinning Weights
		NormalizeSkinningWeights(meshData);
		// Get Root Joint
		aiNode* rootSceneNode = scene->mRootNode;
		for (unsigned int i = 0; i < rootSceneNode->mNumChildren; i++)
		{
			aiNode* aiChildNode = rootSceneNode->mChildren[i];
			std::string nodeName{ aiChildNode->mName.C_Str() };
			if (skeleton.GetJointExists(nodeName))
			{
				CHROMA_TRACE("MODEL LOADER :: Root Joint Found : {0}", aiChildNode->mName.C_Str());
				skeleton.SetRootJointID(skeleton.GetJointID(nodeName));
			}
		}

		// Processing Child and Parent Joints
		for (std::pair<std::string, Joint*> namedJoint : skeleton.GetNamedJoints())
		{
			// If node exists in aiScene, collect child joints 
			// if they are already in the skeleton
			aiNode* jointNode = rootSceneNode->FindNode(aiString(namedJoint.first));
			if (jointNode != NULL)
			{
				// Get Parent ID
				int parentJointID{ -1 };
				GetParentJointID(jointNode, skeleton, parentJointID);
				namedJoint.second->m_ParentJointID = parentJointID;
				// Get Child IDs
				std::vector<int> childJointIDs;
				GetChildJointIDs(jointNode, skeleton, childJointIDs);
				namedJoint.second->m_ChildJointIDs = childJointIDs;
			}
		}
		// Calculate Local Bind Offset Transforms
		skeleton.InitializeSkeleton();
	}

	void ModelLoader::NormalizeSkinningWeights(MeshData& meshData)
	{
		for (ChromaSkinnedVertex& skinnedVert : meshData.skinnedVerts)
			skinnedVert.m_jointWeights = glm::normalize(skinnedVert.m_jointWeights);
	}

	void ModelLoader::GetChildJointIDs(const aiNode* node, Skeleton& skeleton, std::vector<int> & childJointIDs)
	{
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			// Add childnode if found in Skeleton
			std::string childNodeName{ node->mChildren[i]->mName.C_Str() };
			if (skeleton.GetJointExists(childNodeName))
			{
				CHROMA_TRACE("MODEL LOADER :: Adding Child Joint : {0}  : ID : {1} ", childNodeName, skeleton.GetJointID(childNodeName));
				childJointIDs.push_back(skeleton.GetJointID(childNodeName));
			}
			else // Else keep looking
			{
				GetChildJointIDs(node->mChildren[i], skeleton, childJointIDs);
			}
		}
	}

	void ModelLoader::GetParentJointID(const aiNode * node, Skeleton & skeleton, int& parentJointID)
	{
		if (node->mParent != NULL)
		{
			std::string currentJointName{ node->mName.C_Str() };
			std::string parentNodeName{ node->mParent->mName.C_Str() };

			if (skeleton.GetJointExists(parentNodeName))// Set Parent Joint ID if found in skeleton
			{
				parentJointID = skeleton.GetJointID(parentNodeName);
			}
			else // Else keep looking
			{
				GetParentJointID(node->mParent, skeleton, parentJointID);
			}
		}
		else
		{
			parentJointID = -1;
		}
	}
}