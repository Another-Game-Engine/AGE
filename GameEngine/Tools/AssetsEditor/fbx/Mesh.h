#ifndef MESH_H_
#define MESH_H_

#include "Base.h"
#include "Object.h"
#include "MeshPart.h"
#include "VertexElement.h"
#include "BoundingVolume.h"
#include <unordered_map>

struct KeyFuncs
{
    size_t operator()(const gameplay::Vertex& k)const
    {
		return std::hash<float>()(k.position.x) ^ std::hash<float>()(k.position.y) ^ std::hash<float>()(k.position.z)
			^ std::hash<float>()(k.normal.x) ^ std::hash<float>()(k.normal.y) ^ std::hash<float>()(k.normal.z)
			^ std::hash<float>()(k.tangent.x) ^ std::hash<float>()(k.tangent.y) ^ std::hash<float>()(k.tangent.z)
			^ std::hash<float>()(k.binormal.x) ^ std::hash<float>()(k.binormal.y) ^ std::hash<float>()(k.binormal.z)
			^ std::hash<float>()(k.diffuse.x) ^ std::hash<float>()(k.diffuse.y) ^ std::hash<float>()(k.diffuse.z)
			^ std::hash<float>()(k.blendWeights.x) ^ std::hash<float>()(k.blendWeights.y) ^ std::hash<float>()(k.blendWeights.z)
			^ std::hash<float>()(k.blendIndices.x) ^ std::hash<float>()(k.blendIndices.y) ^ std::hash<float>()(k.blendIndices.z);
    }

    bool operator()(const gameplay::Vertex& a, const gameplay::Vertex& b)const
    {
		return a.position == b.position && a.normal == b.normal && a.tangent == b.tangent && a.binormal == b.binormal && a.diffuse == b.diffuse && a.blendWeights == b.blendWeights && a.blendIndices == b.blendIndices;
    }
};

namespace gameplay
{

class Model;

class Mesh : public Object
{
    friend class Model;

public:

    /**
     * Constructor.
     */
    Mesh(void);

    /**
     * Destructor.
     */
    virtual ~Mesh(void);

    virtual unsigned int getTypeId(void) const;
    virtual const char* getElementName(void) const;

    virtual void writeBinary(FILE* file);
    void writeBinaryVertices(FILE* file);

    virtual void writeText(FILE* file);
    void writeText(FILE* file, const Vertex& vertex);
    void writeText(FILE* file, const glm::vec3& v);

    void addMeshPart(MeshPart* part);
    void addMeshPart(Vertex* vertex);
    void addVetexAttribute(unsigned int usage, unsigned int count);

    size_t getVertexCount() const;
    const Vertex& getVertex(unsigned int index) const;

    size_t getVertexElementCount() const;
    const VertexElement& getVertexElement(unsigned int index) const;

    /**
     * Returns true if this MeshPart contains the given Vertex.
     */
    bool contains(const Vertex& vertex) const;

    /**
     * Adds a vertex to this MeshPart and returns the index.
     */
    unsigned int addVertex(const Vertex& vertex);

    unsigned int getVertexIndex(const Vertex& vertex);

    bool hasNormals() const;
    bool hasVertexColors() const;

    void computeBounds();

    Model* model;
    std::vector<Vertex> vertices;
    std::vector<MeshPart*> parts;
    BoundingVolume bounds;
    std::unordered_map<Vertex, unsigned int, KeyFuncs, KeyFuncs> vertexLookupTable;

    std::vector<VertexElement> _vertexFormat;

};

}

#endif
