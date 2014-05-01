#ifndef BOUNDINGVOLUME_H_
#define BOUNDINGVOLUME_H_

#include <glm/glm.hpp>

namespace gameplay
{

/**
 * Represents a 3D bounding volumes, which defines both a
 * bounding sphere and an axis-aligned bounding box (AABB).
 */
class BoundingVolume
{
public:

    /**
     * Radius of the bounding sphere.
     */
    float radius;

    /**
     * Center point of the bounding sphere.
     */
    glm::vec3 center;

    /**
     * Minimum point of the AABB.
     */
    glm::vec3 min;

    /**
     * Maximum point of the AABB.
     */
    glm::vec3 max;

    /**
     * Constructor.
     */
    BoundingVolume();

    /**
     * Transforms this bounding volume by the specified matrix.
     */
    void transform(const glm::mat4& m);

    /**
     * Merges this bounding volume with the specified one and
     * stores the result in this BoundingVolume.
     */
    void merge(const BoundingVolume& v);
};

}

#endif
