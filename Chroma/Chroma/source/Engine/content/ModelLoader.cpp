#include "ModelLoader.h"


namespace Chroma 
{

	// LOADING
	std::vector<MeshComponent*> ModelLoader::Load(std::string const& sourcePath)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(sourcePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			CHROMA_WARN("ERROR::ASSIMP:: {0}", importer.GetErrorString());
			return;
		}

		std::vector<MeshComponent*> meshComponents;
		GetChildMeshNodes(scene->mRootNode, scene, meshComponents);
		return meshComponents;
	}

	void ModelLoader::GetChildMeshNodes(aiNode* node, const aiScene*& scene, std::vector<MeshComponent*>& meshList)
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

	MeshComponent* ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		MeshData newMeshData;

		std::vector<unsigned int> m_Indices;
		std::vector<Texture> m_Textures;

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
			tangent.x = mesh->mTangents[i].x;
			tangent.y = mesh->mTangents[i].y;
			tangent.z = mesh->mTangents[i].z;

			// bitangents 
			glm::vec3 bitangent;
			bitangent.x = mesh->mBitangents[i].x;
			bitangent.y = mesh->mBitangents[i].y;
			bitangent.z = mesh->mBitangents[i].z;

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
				vertex.SetPosition(position);
				vertex.SetNormal(normal);
				vertex.SetTangent(tangent);
				vertex.SetBitangent(bitangent);
				vertex.SetTexCoords(UV1);
				newMeshData.skinnedVerts.push_back(vertex);
			}
			else
			{
				ChromaVertex vertex;
				vertex.SetPosition(position);
				vertex.SetNormal(normal);
				vertex.SetTangent(tangent);
				vertex.SetBitangent(bitangent);
				vertex.SetTexCoords(UV1);
				newMeshData.verts.push_back(vertex);
			}
		}

		// process indices (faces)
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				m_Indices.push_back(face.mIndices[j]);
		}

		// process material, fetching textures
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			// diffuse textures
			std::vector<Texture> diffuseMaps = LoadMaterialTextures(material,
				aiTextureType_DIFFUSE, Texture::ALBEDO);
			m_Textures.insert(m_Textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			// specular textures
			std::vector<Texture> specularMaps = LoadMaterialTextures(material,
				aiTextureType_SPECULAR, Texture::METALNESS);
			m_Textures.insert(m_Textures.end(), specularMaps.begin(), specularMaps.end());
			// normal textures
			std::vector<Texture> normalMaps = LoadMaterialTextures(material,
				aiTextureType_HEIGHT, Texture::NORMAL);
			m_Textures.insert(m_Textures.end(), normalMaps.begin(), normalMaps.end());
		}

		// build Mesh
		if (newMeshData.isSkinned)
		{
			// Process Skeleton
			Skeleton skeleton;
			// Process Joint Hierarchy
			ProcessSkeleton(scene, mesh, skeleton);
			// Skinned Mesh Constructor
			return new SkinnedMesh(newMeshData);
		}
		else
			return new StaticMesh(newMeshData);
	}

	std::vector<Texture> ModelLoader::LoadMaterialTextures(aiMaterial * mat, aiTextureType type, Texture::TYPE typeName)
	{
		std::vector<Texture> m_Textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			bool skip{ false };
			for (unsigned int j = 0; j < m_Textures.size(); j++)
			{
				if (std::strcmp(m_Textures[j].path.data(), str.C_Str()) == 0)
				{
					m_Textures.push_back(m_Textures[j]);
					skip = true;
					break;
				}
			}
			if (!skip)
			{
				Texture texture;
				texture.loadFromFile(str.C_Str(), m_directory);
				texture.type = typeName;
				m_Textures.push_back(texture);
				m_Textures.push_back(texture);
			}
		}
		return m_Textures;
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

	void ModelLoader::ProcessSkeleton(const aiScene * scene, const aiMesh * mesh, Skeleton & skeleton)
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
				SetVertSkinningData(m_SkinnedVertices[vertexWeight.mVertexId], skinningData);
			}
			// Add new joint
			skeleton.AddJoint(newJoint);
		}
		// Normalize Skinning Weights
		NormalizeSkinningWeights();
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

	void ModelLoader::NormalizeSkinningWeights()
	{
		for (ChromaSkinnedVertex& skinnedVert : m_SkinnedVertices)
			skinnedVert.m_jointWeights = glm::normalize(skinnedVert.m_jointWeights);
	}

	void ModelLoader::GetChildJointIDs(aiNode * node, Skeleton & skeleton, std::vector<int> & childJointIDs)
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