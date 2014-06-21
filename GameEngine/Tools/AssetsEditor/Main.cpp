#include <Convertor/SkeletonLoader.hpp>

int main(void)
{
	AGE::AssetDataSet dataSet;
	dataSet.filePath = File("../../Assets/catwoman/atk close front 6.fbx");
	dataSet.name = "catwoman";
	auto skeleton = AGE::SkeletonLoader::load(dataSet);
	return 0;
}