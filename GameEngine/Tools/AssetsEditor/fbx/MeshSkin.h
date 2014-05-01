#ifndef MESHSKIN_H_
#define MESHSKIN_H_

#include "Base.h"
#include "Object.h"
#include <glm/glm.hpp>
#include "Animation.h"
#include "BoundingVolume.h"

namespace gameplay
{

class Node;
class Mesh;

class MeshSkin : public Object
{
    friend class Model;

public:

    /**
     * Constructor.
     */
    MeshSkin(void);

    /**
     * Destructor.
     */
    virtual ~MeshSkin(void);

    virtual unsigned int getTypeId(void) const;
    virtual const char* getElementName(void) const;
    virtual void writeBinary(FILE* file);
    virtual void writeText(FILE* file);

    unsigned int getJointCount() const;

    void setBindShape(const glm::mat4 &m);

    void setVertexInfluenceCount(unsigned int count);

    const std::vector<std::string>& getJointNames();

    void setJointNames(const std::vector<std::string>& list);

    const std::vector<Node*>& getJoints() const;

    void setJoints(const std::vector<Node*>& list);

    void setBindPoses(std::vector<glm::mat4>& list);

    /**
     * Returns true if the MeshSkin contains a joint with the given ID.
     * 
     * @param id The ID of the joint to search for.
     * 
     * @return True if the joint belongs to this skin, false otherwise.
     */
    bool hasJoint(const char* id);

    void computeBounds();

private:

    Mesh* _mesh;
    glm::mat4 _bindShape;
    std::vector<Node*> _joints;
    std::vector<glm::mat4> _bindPoses;
    std::vector<std::string> _jointNames;
    unsigned int _vertexInfluenceCount;
    std::vector<BoundingVolume> _jointBounds;
};

}

#endif
