#include "Scene\SceneLoader.hpp"

void main(void)
{
	AGE::SceneLoader loader("humanoid.fbx");
	auto loadStatus = loader.load();
}