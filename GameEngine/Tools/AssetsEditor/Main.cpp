#include <Convertor/SkeletonLoader.hpp>
#include <Convertor/AnimationsLoader.hpp>

int main(void)
{
	AGE::AssetDataSet dataSet;
	dataSet.filePath = File("../../Assets/catwoman/atk close front 6.fbx");
	dataSet.name = "catwoman";
	auto skeleton = AGE::SkeletonLoader::load(dataSet);
	auto animations = AGE::AnimationsLoader::load(dataSet);
	return 0;
}