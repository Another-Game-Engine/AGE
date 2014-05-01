#ifndef VERTEX_H_
#define VERTEX_H_

#include "FileIO.h"
#include <glm/glm.hpp>

// Maximum number of supported UV sets
#define MAX_UV_SETS 8

static bool isLessThan(const glm::vec2 &a, const glm::vec2 &b)
{
	auto r = glm::lessThan(a, b);

	if (r.x)
		return r.x;
	return r.y;
}

static bool isLessThan(const glm::vec3 &a, const glm::vec3 &b)
{
	auto r = glm::lessThan(a, b);

	if (r.x)
		return r.x;
	if (r.y)
		return r.y;
	return r.z;
}

static bool isLessThan(const glm::vec4 &a, const glm::vec4 &b)
{
	auto r = glm::lessThan(a, b);

	if (r.x)
		return r.x;
	if (r.y)
		return r.y;
	if (r.z)
		return r.z;
	return r.w;
}


namespace gameplay
{

class Vertex
{
public:

    static const unsigned int POSITION_COUNT = 3;
    static const unsigned int NORMAL_COUNT = 3;
    static const unsigned int TANGENT_COUNT = 3;
    static const unsigned int BINORMAL_COUNT = 3;
    static const unsigned int TEXCOORD_COUNT = 2;
    static const unsigned int DIFFUSE_COUNT = 4;
    static const unsigned int BLEND_WEIGHTS_COUNT = 4;
    static const unsigned int BLEND_INDICES_COUNT = 4;

    /**
     * Constructor.
     */
    Vertex(void);

    /**
     * Destructor.
     */
    ~Vertex(void);

    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 binormal;
    glm::vec2 texCoord[MAX_UV_SETS];
    glm::vec4 diffuse;

    glm::vec4 blendWeights;
    glm::vec4 blendIndices;

    bool hasNormal, hasTangent, hasBinormal, hasTexCoord[MAX_UV_SETS], hasDiffuse, hasWeights;

    inline bool operator<(const Vertex& v) const
    {
        if (position == v.position)
        {
            if (normal == v.normal)
            {
                if (tangent == v.tangent)
                {
                    if (binormal == v.binormal)
                    {
                        if (diffuse == v.diffuse)
                        {
                            if (blendWeights == v.blendWeights)
                            {
                                if (blendIndices == v.blendIndices)
                                {
                                    for (unsigned int i = 0; i < MAX_UV_SETS; ++i)
                                    {
										if (!(texCoord[i] == v.texCoord[i]))
											return isLessThan(texCoord[i], v.texCoord[i]);
                                    }
                                    return false;
                                }
								return isLessThan(blendIndices, v.blendIndices);
                            }
							return isLessThan(blendWeights, v.blendWeights);
                        }
						return isLessThan(diffuse, v.diffuse);
                    }
					return isLessThan(binormal, v.binormal);
                }
				return isLessThan(tangent, v.tangent);
            }
			return isLessThan(normal, v.normal);
        }
		return isLessThan(position, v.position);
    }

    inline bool operator==(const Vertex& v) const
    {
        for (unsigned int i = 0; i < MAX_UV_SETS; ++i)
        {
            if (!(texCoord[i] == v.texCoord[i]))
                return false;
        }
        return position==v.position && normal==v.normal && tangent==v.tangent && binormal==v.binormal &&
            diffuse==v.diffuse && blendWeights==v.blendWeights && blendIndices==v.blendIndices;
    }

    /**
     * Returns the size of this vertex in bytes.
     */
    unsigned int byteSize() const;

    /**
     * Writes this vertex to the binary file stream.
     */
    void writeBinary(FILE* file) const;

    /**
     * Writes this vertex to a text file stream.
     */
    void writeText(FILE* file) const;

    /**
     * Normalizes the blend weights of this vertex so that they add up to 1.0.
     */
    void normalizeBlendWeight();
};
}

#endif
