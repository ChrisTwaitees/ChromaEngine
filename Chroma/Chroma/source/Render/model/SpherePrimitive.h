#ifndef _CHROMA_SPHERE_
#define _CHROMA_SPHERE_

//common
#include <common/PrecompiledHeader.h>

#include <model/StaticMesh.h>


class SpherePrimitive : public Chroma::StaticMesh
{
public:
	// ctor/dtor
	SpherePrimitive(float m_Radius = 1.0f, int sectorCount = 36, int stackCount = 18);
	~SpherePrimitive() {}

	// getters/setters
	float getRadius() const { return m_Radius; }
	int getSectorCount() const { return sectorCount; }
	int getStackCount() const { return stackCount; }
	void set(float m_Radius, int sectorCount, int stackCount);
	void setRadius(float m_Radius);
	void setSectorCount(int sectorCount);
	void setStackCount(int stackCount);

	// for vertex data
	unsigned int getVertexCount() const { return (unsigned int)m_verts.size() / 3; }
	unsigned int getNormalCount() const { return (unsigned int)normals.size() / 3; }
	unsigned int getTexCoordCount() const { return (unsigned int)texCoords.size() / 2; }
	unsigned int getIndexCount() const { return (unsigned int)m_Indices.size(); }
	unsigned int getLineIndexCount() const { return (unsigned int)lineIndices.size(); }
	unsigned int getTriangleCount() const { return getIndexCount() / 3; }
	unsigned int getVertexSize() const { return (unsigned int)m_verts.size() * sizeof(float); }
	unsigned int getNormalSize() const { return (unsigned int)normals.size() * sizeof(float); }
	unsigned int getTexCoordSize() const { return (unsigned int)texCoords.size() * sizeof(float); }
	unsigned int getIndexSize() const { return (unsigned int)m_Indices.size() * sizeof(unsigned int); }
	unsigned int getLineIndexSize() const { return (unsigned int)lineIndices.size() * sizeof(unsigned int); }

	std::vector<ChromaVertex> GetVertices() override { return m_MeshData.verts; };
	const float* getNormals() const { return normals.data(); }
	const float* getTexCoords() const { return texCoords.data(); }
	const unsigned int* getIndices() const { return m_Indices.data(); }
	const unsigned int* getLineIndices() const { return lineIndices.data(); }

	// for interleaved vertices: V/N/T
	unsigned int getInterleavedVertexCount() const { return getVertexCount(); }    // # of vertices
	unsigned int getInterleavedVertexSize() const { return (unsigned int)interleavedVertices.size() * sizeof(float); }    // # of bytes
	int getInterleavedStride() const { return interleavedStride; }   // should be 32 bytes
	const float* getInterleavedVertices() const { return interleavedVertices.data(); }

	// overrides
	void SetupMesh() override;
	void BindDrawVAO() override;


protected:

private:
	// member functions
	void updateRadius();
	void buildVerticesSmooth();
	void buildInterleavedVertices();
	void clearArrays();
	void addVertex(float x, float y, float z);
	void addNormal(float x, float y, float z);
	void addTexCoord(float s, float t);
	void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);
	std::vector<float> computeFaceNormal(float x1, float y1, float z1,
		float x2, float y2, float z2,
		float x3, float y3, float z3);

	// memeber vars
	float m_Radius;
	int sectorCount;                        // longitude, # of slices
	int stackCount;                         // latitude, # of stacks
	bool smooth;
	std::vector<float> m_verts;
	std::vector<float> normals;
	std::vector<float> texCoords;
	std::vector<unsigned int> m_Indices;
	std::vector<unsigned int> lineIndices;
	std::vector<ChromaVertex> m_vertices;

	// interleaved
	std::vector<float> interleavedVertices;
	int interleavedStride;                  // # of bytes to hop to the next vertex (should be 32 bytes)
};


#endif