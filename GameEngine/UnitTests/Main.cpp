
#include <stdlib.h>
#include <Core/OctreeNode.hh>
#include <Core/CullableInterfaces.hh>

static void	unitTestOctree()
{
	// Octree node
	AGE::OctreeNode				*root = new AGE::OctreeNode;
	// Cullable objects
	AGE::CullableBoundingBox	*box = new AGE::CullableBoundingBox;
	AGE::CullableFrustum		*frustum = new AGE::CullableFrustum;
	AGE::CullableSphere			*sphere = new AGE::CullableSphere;

	// Init the CullableBox
	AGE::AABoundingBox			aabb(glm::vec3(-1), glm::vec3(1));
	glm::mat4					transform = glm::translate(glm::mat4(1), glm::vec3(-1));

	box->currentAABB.fromTransformedBox(aabb, transform);

	// The unit tests
	std::cout << "- Basic extend test:" << std::endl;
	root = root->AddElement(box);
	if (root->getNodeBoundingBox().minPoint == glm::vec3(-3) &&
		root->getNodeBoundingBox().maxPoint == glm::vec3(1))
		std::cout << "\tSUCCEED" << std::endl;
	else
		std::cout << "\tFAILED" << std::endl;
}

int		main(int argc, char*argv[])
{
	unitTestOctree();
	return (EXIT_SUCCESS);
}