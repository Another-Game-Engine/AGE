#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_interpolation.hpp>

#include <Utils/Containers/Vector.hpp>
#include <map>
#include <chrono>

#include <Core/Engine.hh>
#include <Core/SceneManager.hh>

// DEPENDENCIES
#include <Context/SdlContext.hh>
#include <Core/ConfigurationManager.hpp>
#include <Physic/BulletDynamicManager.hpp>
#include <Core/Timer.hh>
#include <Utils/PubSub.hpp>
#include <Render/GeometryManager.hh>
#include <Utils/PerformanceDebugger.hh>
#include <Core/AssetsManager.hpp>

#include <Convertor/SkeletonLoader.hpp>
#include <Convertor/AnimationsLoader.hpp>
#include <Convertor/MeshLoader.hpp>
#include <Convertor/MaterialConvertor.hpp>
#include <Convertor/ImageLoader.hpp>

//SKINNING
#include <Skinning/Animation.hpp>
#include <Skinning/AnimationChannel.hpp>
#include <Skinning/AnimationInstance.hpp>
#include <Skinning/AnimationKey.hpp>
#include <Skinning/Bone.hpp>
#include <Skinning/Skeleton.hpp>


int			main(int ac, char **av)
{
	{
		AGE::AssetDataSet dataSet;
		dataSet.filePath = File("ball/ball.obj");
		dataSet.skinName = "ball";
		dataSet.materialName = "ball";

		dataSet.serializedDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Serialized");
		dataSet.rawDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Raw");

		AGE::MaterialLoader::load(dataSet);
		AGE::ImageLoader::load(dataSet);
		AGE::MeshLoader::load(dataSet);

		AGE::MaterialLoader::save(dataSet);
		AGE::ImageLoader::save(dataSet);
		AGE::MeshLoader::save(dataSet);
	}
	{
		AGE::AssetDataSet dataSet;
		dataSet.filePath = File("cube/cube.obj");
		dataSet.skinName = "cube";
		dataSet.materialName = "cube";

		dataSet.serializedDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Serialized");
		dataSet.rawDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Raw");

		AGE::MaterialLoader::load(dataSet);
		AGE::ImageLoader::load(dataSet);
		AGE::MeshLoader::load(dataSet);

		AGE::MaterialLoader::save(dataSet);
		AGE::ImageLoader::save(dataSet);
		AGE::MeshLoader::save(dataSet);
	}
	return (0);
}