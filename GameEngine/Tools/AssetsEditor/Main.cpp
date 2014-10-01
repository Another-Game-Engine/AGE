#include <Utils/Containers/Vector.hpp>
#include <map>

//EDITOR
#include "AssetConvertor.hh"
#include "SimpleConf.hh"

//CONVERTOR
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
		AGE::SimpleConf confFile("convertor.conf");
		AGE::AssetConvertor convertor(&confFile);
		convertor.loads();
		convertor.saves();
	}
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
		dataSet.filePath = File("catwoman/catwoman.fbx");
		dataSet.skeletonName = "catwoman";
		dataSet.animationName = "catwoman-roulade";
		dataSet.skinName = "catwoman";
		dataSet.materialName = "catwoman";

		dataSet.serializedDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Serialized");
		dataSet.rawDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Raw");

		AGE::MaterialLoader::load(dataSet);
		AGE::ImageLoader::load(dataSet);
		AGE::SkeletonLoader::load(dataSet);
		AGE::AnimationsLoader::load(dataSet);
		AGE::MeshLoader::load(dataSet);

		AGE::MaterialLoader::save(dataSet);
		AGE::ImageLoader::save(dataSet);
		AGE::MeshLoader::save(dataSet);
		AGE::SkeletonLoader::save(dataSet);
		AGE::AnimationsLoader::save(dataSet);
	}
	{
		AGE::AssetDataSet dataSet;
		dataSet.filePath = File("head/head.obj");
		dataSet.skinName = "head";
		dataSet.materialName = "head";

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