#include <Render/GeometryManagement/IcoSphere.hh>

#include <unordered_map>

namespace AGE
{
	typedef std::pair<uint32_t, uint32_t>				idxPair_t;

	class IndexPairHash
	{
	public:
		std::size_t operator ()(const idxPair_t &idxpair) const
		{
			uint32_t a = idxpair.first;
			uint32_t b = idxpair.second;
			return (a >= b ? a * a + a + b : a + b * b);
		}
	};

	typedef std::unordered_map<idxPair_t, uint32_t, IndexPairHash>		idxHash_t;

	static uint32_t getMiddlePoint(std::vector<glm::vec3> vertexTab, idxHash_t &middlePoints, uint32_t p1, uint32_t p2)
	{
		// first check if we have it already
		bool firstIsSmaller = p1 < p2;
		uint32_t smallerIndex = firstIsSmaller ? p1 : p2;
		uint32_t greaterIndex = firstIsSmaller ? p2 : p1;
		idxPair_t key;

		key.first = smallerIndex;
		key.second = greaterIndex;

		uint32_t ret;

		idxHash_t::iterator it = middlePoints.find(key);
		if (it != middlePoints.end())
		{
			return it->second;
		}

		// not in cache, calculate it
		glm::vec3 point1 = vertexTab[p1];
		glm::vec3 point2 = vertexTab[p2];
		glm::vec3 middle = (point1 + point2) / 2.0f;

		// add vertex makes sure point is on unit sphere
		vertexTab.push_back(glm::normalize(glm::vec3(middle)));

		ret = vertexTab.size() - 1;

		// store it, return index
		middlePoints[key] = ret;
		return ret;
	}

	void generateIcoSphere(uint32_t recursion, std::vector<glm::vec3> &vertex, std::vector<glm::u32vec3> &indices)
	{
		idxHash_t					middlePoints;
		uint32_t					currentIdx = 0;

		// create 12 vertices of a icosahedron
		float t = (1.0 + std::sqrt(5.0)) / 2.0;

		vertex.push_back(glm::normalize(glm::vec3(-1, t, 0)));
		vertex.push_back(glm::normalize(glm::vec3(1, t, 0)));
		vertex.push_back(glm::normalize(glm::vec3(-1, -t, 0)));
		vertex.push_back(glm::normalize(glm::vec3(1, -t, 0)));

		vertex.push_back(glm::normalize(glm::vec3(0, -1, t)));
		vertex.push_back(glm::normalize(glm::vec3(0, 1, t)));
		vertex.push_back(glm::normalize(glm::vec3(0, -1, -t)));
		vertex.push_back(glm::normalize(glm::vec3(0, 1, -t)));

		vertex.push_back(glm::normalize(glm::vec3(t, 0, -1)));
		vertex.push_back(glm::normalize(glm::vec3(t, 0, 1)));
		vertex.push_back(glm::normalize(glm::vec3(-t, 0, -1)));
		vertex.push_back(glm::normalize(glm::vec3(-t, 0, 1)));

		indices.push_back(glm::u32vec3(0, 11, 5));
		indices.push_back(glm::u32vec3(0, 5, 1));
		indices.push_back(glm::u32vec3(0, 1, 7));
		indices.push_back(glm::u32vec3(0, 7, 10));
		indices.push_back(glm::u32vec3(0, 10, 11));

		// 5 adjacent faces 
		indices.push_back(glm::u32vec3(1, 5, 9));
		indices.push_back(glm::u32vec3(5, 11, 4));
		indices.push_back(glm::u32vec3(11, 10, 2));
		indices.push_back(glm::u32vec3(10, 7, 6));
		indices.push_back(glm::u32vec3(7, 1, 8));

		// 5 faces around point 3
		indices.push_back(glm::u32vec3(3, 9, 4));
		indices.push_back(glm::u32vec3(3, 4, 2));
		indices.push_back(glm::u32vec3(3, 2, 6));
		indices.push_back(glm::u32vec3(3, 6, 8));
		indices.push_back(glm::u32vec3(3, 8, 9));

		// 5 adjacent faces 
		indices.push_back(glm::u32vec3(4, 9, 5));
		indices.push_back(glm::u32vec3(2, 4, 11));
		indices.push_back(glm::u32vec3(6, 2, 10));
		indices.push_back(glm::u32vec3(8, 6, 7));
		indices.push_back(glm::u32vec3(9, 8, 1));

		// refine triangles
		for (int i = 0; i < recursion; i++)
		{
			std::vector<glm::u32vec3>			idTab2;

			for (auto indices : indices)
			{
				// replace triangle by 4 triangles
				uint32_t a = getMiddlePoint(vertex, middlePoints, indices.x, indices.y);
				uint32_t b = getMiddlePoint(vertex, middlePoints, indices.y, indices.z);
				uint32_t c = getMiddlePoint(vertex, middlePoints, indices.z, indices.x);

				idTab2.push_back(glm::u32vec3(indices.x, a, c));
				idTab2.push_back(glm::u32vec3(indices.y, b, a));
				idTab2.push_back(glm::u32vec3(indices.z, c, b));
				idTab2.push_back(glm::u32vec3(a, b, c));
			}
			indices = idTab2;
		}
	}
}
