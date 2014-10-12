
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
	std::cout << "- Basic AABB extend test:" << std::endl;
	root = root->addElement(box);
	if (root->getNodeBoundingBox().minPoint == glm::vec3(-3) &&
		root->getNodeBoundingBox().maxPoint == glm::vec3(1))
		std::cout << "\tSUCCEED" << std::endl;
	else
		std::cout << "\tFAILED" << std::endl;
	std::cout << "- Basic AABB remove test:" << std::endl;
	root = root->removeElement(box);
	if (root->isLeaf())
		std::cout << "\tSUCCEED" << std::endl;
	else
		std::cout << "\tFAILED" << std::endl;
	// change the size of the aabb
	aabb.minPoint = glm::vec3(0);
	aabb.maxPoint = glm::vec3(0.01);
	// generate 100 boxes
	AGE::CullableBoundingBox	*boxes[100];
	std::cout << "- Basic node split test:" << std::endl;
	for (uint32_t i = 0; i < 100; ++i)
	{
		boxes[i] = new AGE::CullableBoundingBox;
		boxes[i]->currentAABB.fromTransformedBox(aabb, glm::translate(glm::mat4(1), glm::vec3(i * 0.01)));
		root = root->addElement(boxes[i]);
	}
	if (root->isLeaf() == false)
		std::cout << "\tSUCCEED" << std::endl;
	else
		std::cout << "\tFAILED" << std::endl;
}

int		main(int argc, char*argv[])
{
	unitTestOctree();
	return (EXIT_SUCCESS);
}