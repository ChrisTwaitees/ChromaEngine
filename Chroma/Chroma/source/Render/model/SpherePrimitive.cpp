#include "SpherePrimitive.h"

// constants //////////////////////////////////////////////////////////////////
const int MIN_SECTOR_COUNT = 3;
const int MIN_STACK_COUNT = 2;



///////////////////////////////////////////////////////////////////////////////
// ctor
///////////////////////////////////////////////////////////////////////////////
SpherePrimitive::SpherePrimitive(float radius, int sectors, int stacks) : interleavedStride(32)
{
	set(radius, sectors, stacks);
	SetupMesh();
}



///////////////////////////////////////////////////////////////////////////////
// setters
///////////////////////////////////////////////////////////////////////////////
void SpherePrimitive::set(float radius, int sectors, int stacks)
{
	this->radius = radius;
	this->sectorCount = sectors;
	if (sectors < MIN_SECTOR_COUNT)
		this->sectorCount = MIN_SECTOR_COUNT;
	this->stackCount = stacks;
	if (sectors < MIN_STACK_COUNT)
		this->sectorCount = MIN_STACK_COUNT;

	buildVerticesSmooth();

}

void SpherePrimitive::setRadius(float radius)
{
	this->radius = radius;
	updateRadius();
}

void SpherePrimitive::setSectorCount(int sectors)
{
	set(radius, sectors, stackCount);
}

void SpherePrimitive::setStackCount(int stacks)
{
	set(radius, sectorCount, stacks);
}


void SpherePrimitive::SetupMesh()
{
	// Vertex Array Object Buffer
	glGenVertexArrays(1, &VAO);
	// copy interleaved vertex data (V/N/T) to VBO
	glGenBuffers(1, &VBO);
	// copy index data to VBO
	glGenBuffers(1, &EBO);

	// Bind buffers
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);           // for vertex data
	glBufferData(GL_ARRAY_BUFFER,                   // target
		getInterleavedVertexSize(), // data size, # of bytes
		getInterleavedVertices(),   // ptr to vertex data
		GL_STATIC_DRAW);                   // usage

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);   // for index data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,           // target
		getIndexSize(),             // data size, # of bytes
		getIndices(),               // ptr to index data
		GL_STATIC_DRAW);                   // usage


	// set attrib arrays with stride and m_Offset
	int stride = getInterleavedStride();     // should be 32 bytes
	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, stride, (void*)(sizeof(float) * 3));
	// vertex uvs
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, stride, (void*)(sizeof(float) * 6));


	glBindVertexArray(0);
}

void SpherePrimitive::BindDrawVAO()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, getIndexCount(), GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0); // reset to default
}


///////////////////////////////////////////////////////////////////////////////
// update vertex positions only
///////////////////////////////////////////////////////////////////////////////
void SpherePrimitive::updateRadius()
{
	float scale = sqrtf(radius * radius / (m_verts[0] * m_verts[0] + m_verts[1] * m_verts[1] + m_verts[2] * m_verts[2]));

	std::size_t i, j;
	std::size_t count = m_verts.size();
	for (i = 0, j = 0; i < count; i += 3, j += 8)
	{
		m_verts[i] *= scale;
		m_verts[i + 1] *= scale;
		m_verts[i + 2] *= scale;

		// for interleaved array
		interleavedVertices[j] *= scale;
		interleavedVertices[j + 1] *= scale;
		interleavedVertices[j + 2] *= scale;
	}
}



///////////////////////////////////////////////////////////////////////////////
// dealloc vectors
///////////////////////////////////////////////////////////////////////////////
void SpherePrimitive::clearArrays()
{
	std::vector<float>().swap(m_verts);
	std::vector<float>().swap(normals);
	std::vector<float>().swap(texCoords);
	std::vector<unsigned int>().swap(m_Indices);
	std::vector<unsigned int>().swap(lineIndices);
}



///////////////////////////////////////////////////////////////////////////////
// build vertices of sphere with smooth shading using parametric equation
// x = r * cos(u) * cos(v)
// y = r * cos(u) * sin(v)
// z = r * sin(u)
// where u: stack(latitude) angle (-90 <= u <= 90)
//       v: sector(longitude) angle (0 <= v <= 360)
///////////////////////////////////////////////////////////////////////////////
void SpherePrimitive::buildVerticesSmooth()
{
	const float PI = 3.1415926f;

	// clear memory of prev arrays
	clearArrays();

	float x, y, z, xy;                              // vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius;    // normal
	float s, t;                                     // texCoord

	float sectorStep = 2 * PI / sectorCount;
	float stackStep = PI / stackCount;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= stackCount; ++i)
	{
		stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
		xy = radius * cosf(stackAngle);             // r * cos(u)
		z = radius * sinf(stackAngle);              // r * sin(u)

		// add (sectorCount+1) vertices per stack
		// the first and last vertices have same position and normal, but different tex coords
		for (int j = 0; j <= sectorCount; ++j)
		{
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi

			// vertex position
			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
			addVertex(x, y, z);

			// normalized vertex normal
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			addNormal(nx, ny, nz);

			// vertex tex coord between [0, 1]
			s = (float)j / sectorCount;
			t = (float)i / stackCount;
			addTexCoord(s, t);
		}
	}

	// indices
	//  k1--k1+1
	//  |  / |
	//  | /  |
	//  k2--k2+1
	unsigned int k1, k2;
	for (int i = 0; i < stackCount; ++i)
	{
		k1 = i * (sectorCount + 1);     // beginning of current stack
		k2 = k1 + sectorCount + 1;      // beginning of next stack

		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding 1st and last stacks
			if (i != 0)
			{
				addIndices(k1, k2, k1 + 1);   // k1---k2---k1+1
			}

			if (i != (stackCount - 1))
			{
				addIndices(k1 + 1, k2, k2 + 1); // k1+1---k2---k2+1
			}

			// vertical lines for all stacks
			lineIndices.push_back(k1);
			lineIndices.push_back(k2);
			if (i != 0)  // horizontal lines except 1st stack
			{
				lineIndices.push_back(k1);
				lineIndices.push_back(k1 + 1);
			}
		}
	}

	// generate interleaved vertex array as well
	buildInterleavedVertices();
}


///////////////////////////////////////////////////////////////////////////////
// generate interleaved vertices: V/N/T
// stride must be 32 bytes
///////////////////////////////////////////////////////////////////////////////
void SpherePrimitive::buildInterleavedVertices()
{
	std::vector<float>().swap(interleavedVertices);
	verts.clear();


	std::size_t i, j;
	std::size_t count = m_verts.size();
	for (i = 0, j = 0; i < count; i += 3, j += 2)
	{
		ChromaVertex newVert;

		interleavedVertices.push_back(m_verts[i]);
		interleavedVertices.push_back(m_verts[i + 1]);
		interleavedVertices.push_back(m_verts[i + 2]);
		newVert.SetPosition(glm::vec3(m_verts[i], m_verts[i+1], m_verts[i+2]));

		interleavedVertices.push_back(normals[i]);
		interleavedVertices.push_back(normals[i + 1]);
		interleavedVertices.push_back(normals[i + 2]);
		newVert.SetNormal(glm::vec3(normals[i], normals[i + 1], normals[i + 2]));

		interleavedVertices.push_back(texCoords[j]);
		interleavedVertices.push_back(texCoords[j + 1]);
		newVert.SetTexCoords(glm::vec2(texCoords[j], texCoords[j + 1]));

		verts.push_back(newVert);
	}
}



///////////////////////////////////////////////////////////////////////////////
// add single vertex to array
///////////////////////////////////////////////////////////////////////////////
void SpherePrimitive::addVertex(float x, float y, float z)
{
	m_verts.push_back(x);
	m_verts.push_back(y);
	m_verts.push_back(z);

}



///////////////////////////////////////////////////////////////////////////////
// add single normal to array
///////////////////////////////////////////////////////////////////////////////
void SpherePrimitive::addNormal(float nx, float ny, float nz)
{
	normals.push_back(nx);
	normals.push_back(ny);
	normals.push_back(nz);
}



///////////////////////////////////////////////////////////////////////////////
// add single texture coord to array
///////////////////////////////////////////////////////////////////////////////
void SpherePrimitive::addTexCoord(float s, float t)
{
	texCoords.push_back(s);
	texCoords.push_back(t);
}



///////////////////////////////////////////////////////////////////////////////
// add 3 indices to array
///////////////////////////////////////////////////////////////////////////////
void SpherePrimitive::addIndices(unsigned int i1, unsigned int i2, unsigned int i3)
{
	m_Indices.push_back(i1);
	m_Indices.push_back(i2);
	m_Indices.push_back(i3);
}



///////////////////////////////////////////////////////////////////////////////
// return face normal of a triangle v1-v2-v3
// if a triangle has no surface (normal length = 0), then return a zero vector
///////////////////////////////////////////////////////////////////////////////
std::vector<float> SpherePrimitive::computeFaceNormal(float x1, float y1, float z1,  // v1
	float x2, float y2, float z2,  // v2
	float x3, float y3, float z3)  // v3
{
	const float EPSILON = 0.000001f;

	std::vector<float> normal(3, 0.0f);     // default return value (0,0,0)
	float nx, ny, nz;

	// find 2 edge vectors: v1-v2, v1-v3
	float ex1 = x2 - x1;
	float ey1 = y2 - y1;
	float ez1 = z2 - z1;
	float ex2 = x3 - x1;
	float ey2 = y3 - y1;
	float ez2 = z3 - z1;

	// cross product: e1 x e2
	nx = ey1 * ez2 - ez1 * ey2;
	ny = ez1 * ex2 - ex1 * ez2;
	nz = ex1 * ey2 - ey1 * ex2;

	// normalize only if the length is > 0
	float length = sqrtf(nx * nx + ny * ny + nz * nz);
	if (length > EPSILON)
	{
		// normalize
		float lengthInv = 1.0f / length;
		normal[0] = nx * lengthInv;
		normal[1] = ny * lengthInv;
		normal[2] = nz * lengthInv;
	}

	return normal;
}