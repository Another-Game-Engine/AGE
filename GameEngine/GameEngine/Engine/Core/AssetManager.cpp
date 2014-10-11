#include <Core/AssetsManager.hpp>
#include <Skinning/Skeleton.hpp>
#include <Skinning/Animation.hpp>
#include <Geometry/Mesh.hpp>
#include <Geometry/Material.hpp>
#include <Texture/Texture.hpp>
#include <Render/RenderManager.hh>

namespace AGE
{
	std::shared_ptr<MeshInstance> AssetsManager::getMesh(const File &_filePath)
	{
		File filePath(_assetsDirectory + _filePath.getFullName());
		if (_meshs.find(filePath.getFullName()) != std::end(_meshs))
			return _meshs[filePath.getFullName()];
		return nullptr;
	}

	std::shared_ptr<MaterialSetInstance> AssetsManager::getMaterial(const File &_filePath)
	{
		File filePath(_assetsDirectory + _filePath.getFullName());
		if (_materials.find(filePath.getFullName()) != std::end(_materials))
			return _materials[filePath.getFullName()];
		return nullptr;
	}


	std::shared_ptr<MaterialSetInstance> AssetsManager::loadMaterial(const File &_filePath)
	{
		File filePath(_assetsDirectory + _filePath.getFullName());

		if (_materials.find(filePath.getFullName()) != std::end(_materials))
			return _materials[filePath.getFullName()];
		if (!filePath.exists())
		{
			std::cerr << "AssetsManager : File [" << filePath.getFullName() << "] does not exists." << std::endl;
			assert(false);
		}

		MaterialDataSet data;
		auto material = std::make_shared<MaterialSetInstance>();

		std::ifstream ifs(filePath.getFullName(), std::ios::binary);
		cereal::PortableBinaryInputArchive ar(ifs);
		ar(data);

		auto manager = _dependencyManager.lock()->getInstance<gl::RenderManager>();
		for (auto &e : data.collection)
		{
			auto key = manager->addMaterial();
			material->datas.push_back(key);

			// TODO fill material with material key
			gl::Key<gl::Material> &mat = material->datas.back();
			manager->setMaterial<gl::Color_diffuse>(mat, e.diffuse);
			manager->setMaterial<gl::Color_emissive>(mat, e.emissive);
			manager->setMaterial<gl::Color_specular>(mat, e.specular);

			manager->setMaterial<gl::Texture_diffuse>(mat, loadTexture(e.diffuseTexPath));
			manager->setMaterial<gl::Texture_emissive>(mat, loadTexture(e.emissiveTexPath));
			manager->setMaterial<gl::Texture_specular>(mat, loadTexture(e.specularTexPath));
			manager->setMaterial<gl::Texture_bump>(mat, loadTexture(e.bumpTexPath));

			manager->setMaterial<gl::Ratio_diffuse>(mat, 1.0f); // todo
			manager->setMaterial<gl::Ratio_emissive>(mat, 1.0f); // todo
			manager->setMaterial<gl::Ratio_specular>(mat, 1.0f); // todo
		}

		_materials.insert(std::make_pair(filePath.getFullName(), material));
		return material;
	}

	gl::Key<gl::Texture> AssetsManager::loadTexture(const File &_filePath)
	{
		auto manager = _dependencyManager.lock()->getInstance<gl::RenderManager>();
		File filePath(_assetsDirectory + _filePath.getFullName());
		if (_textures.find(filePath.getFullName()) != std::end(_textures))
			return _textures[filePath.getFullName()];
		if (!filePath.exists())
		{
			return (manager->getDefaultTexture2D());
		}

		TextureData data;

		std::ifstream ifs(filePath.getFullName(), std::ios::binary);
		cereal::PortableBinaryInputArchive ar(ifs);
		ar(data);

		// TODO fill texture with texture key
		
		GLenum ct = GL_RGB32F;
		GLenum color = GL_RGB;
		if (data.colorNumber == 3)
		{
			ct = GL_RGB32F;
			color = GL_BGR;
		}
		else if (data.colorNumber == 4)
		{
			ct = GL_RGBA32F;
			color = GL_BGRA;
		}
		else if (data.colorNumber == 1)
		{
			ct = GL_R8;
			color = GL_LUMINANCE;
		}
		else
			assert(false);
		auto key = manager->addTexture2D(data.width, data.height, ct, true);

		manager->uploadTexture(key, color, GL_UNSIGNED_BYTE, data.data.data());
		manager->parameterTexture(key, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		manager->parameterTexture(key, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		manager->parameterTexture(key, GL_TEXTURE_WRAP_T, GL_REPEAT);
		manager->parameterTexture(key, GL_TEXTURE_WRAP_S, GL_REPEAT);
		_textures.insert(std::make_pair(filePath.getFullName(), key));
		return key;
	}

	std::shared_ptr<Animation> AssetsManager::loadAnimation(const File &_filePath)
	{
		File filePath(_assetsDirectory + _filePath.getFullName());
		if (_animations.find(filePath.getFullName()) != std::end(_animations))
			return _animations[filePath.getFullName()];
		if (!filePath.exists())
		{
			std::cerr << "AssetsManager : File [" << filePath.getFullName() << "] does not exists." << std::endl;
			assert(false);
		}

		auto animation = std::make_shared<Animation>();

		std::ifstream ifs(filePath.getFullName(), std::ios::binary);
		cereal::PortableBinaryInputArchive ar(ifs);
		ar(*animation.get());
		_animations.insert(std::make_pair(filePath.getFullName(), animation));
		return animation;
	}

	std::shared_ptr<Animation> AssetsManager::getAnimation(const File &_filePath)
	{
		File filePath(_assetsDirectory + _filePath.getFullName());
		if (_animations.find(filePath.getFullName()) != std::end(_animations))
			return _animations[filePath.getFullName()];
		return nullptr;
	}

	std::shared_ptr<Skeleton> AssetsManager::getSkeleton(const File &_filePath)
	{
		File filePath(_assetsDirectory + _filePath.getFullName());
		if (_skeletons.find(filePath.getFullName()) != std::end(_skeletons))
			return _skeletons[filePath.getFullName()];
		return nullptr;
	}

	std::shared_ptr<Skeleton> AssetsManager::loadSkeleton(const File &_filePath)
	{
		File filePath(_assetsDirectory + _filePath.getFullName());
		if (_skeletons.find(filePath.getFullName()) != std::end(_skeletons))
			return _skeletons[filePath.getFullName()];
		if (!filePath.exists())
		{
			std::cerr << "AssetsManager : File [" << filePath.getFullName() << "] does not exists." << std::endl;
			assert(false);
		}

		auto skeleton = std::make_shared<Skeleton>();

		std::ifstream ifs(filePath.getFullName(), std::ios::binary);
		cereal::PortableBinaryInputArchive ar(ifs);
		ar(*skeleton.get());
		_skeletons.insert(std::make_pair(filePath.getFullName(), skeleton));
		return skeleton;
	}

	std::shared_ptr<MeshInstance> AssetsManager::loadMesh(const File &_filePath, const std::vector<MeshInfos> &loadOrder)
	{
		File filePath(_assetsDirectory + _filePath.getFullName());
		if (_meshs.find(filePath.getFullName()) != std::end(_meshs))
			return _meshs[filePath.getFullName()];
		if (!filePath.exists())
		{
			std::cerr << "AssetsManager : File [" << filePath.getFullName() << "] does not exists." << std::endl;
			assert(false);
		}

		std::ifstream ifs(filePath.getFullName(), std::ios::binary);
		cereal::PortableBinaryInputArchive ar(ifs);
		MeshData data;
		ar(data);

		auto meshInstance = std::make_shared<MeshInstance>();

		meshInstance->subMeshs.resize(data.subMeshs.size());
		meshInstance->name = data.name;

		for (std::size_t i = 0; i < data.subMeshs.size(); ++i)
		{
			// If no vertex pool correspond to submesh
			std::vector<MeshInfos> order;
			std::bitset<MeshInfos::END> infos;
			if (loadOrder.empty())
			{
				for (std::size_t j = 0; i < data.subMeshs[i].infos.size(); ++j)
				{
					if (!data.subMeshs[i].infos.test(j))
						continue;
					order.push_back(MeshInfos(j));
					infos.set(j);
				}
			}
			else
			{
				for (auto &e : loadOrder)
				{
					if (!data.subMeshs[i].infos.test(e))
						continue;
					order.push_back(e);
					infos.set(e);
				}
			}
			if (_pools.find(infos) == std::end(_pools))
			{
				createPool(order, infos);
			}
			loadSubmesh(data.subMeshs[i], meshInstance->subMeshs[i], order, infos);
		}
		_meshs.insert(std::make_pair(filePath.getFullName(), meshInstance));
		return meshInstance;
	}

	void AssetsManager::loadSubmesh(SubMeshData &data
		, SubMeshInstance &mesh
		, const std::vector<MeshInfos> &order
		, const std::bitset<MeshInfos::END> &infos)
	{
		auto &pools = _pools.find(infos)->second;
		auto &geometryManager = _dependencyManager.lock()->getInstance<gl::RenderManager>()->geometryManager;

		std::size_t size = data.infos.count();

		AGE::Vector<void*> buffer;
		buffer.resize(size);
		AGE::Vector<std::size_t> nbrBuffer;
		nbrBuffer.resize(size);

		std::size_t ctr = 0;
		auto sizeofFloat = sizeof(float);
		auto maxSize = data.positions.size();

		for (auto &e : order)
		{
			switch (MeshInfos(e))
			{
			case Positions:
				buffer[ctr] = &data.positions[0].x;
				nbrBuffer[ctr] = data.positions.size() * 4 * sizeofFloat;
				if (data.positions.size() > maxSize)
					maxSize = data.positions.size();
				break;
			case Normals:
				buffer[ctr] = &data.normals[0].x;
				nbrBuffer[ctr] = data.normals.size() * 4 * sizeofFloat;
				if (data.normals.size() > maxSize)
					maxSize = data.normals.size();
				break;
			case Tangents:
				buffer[ctr] = &data.tangents[0].x;
				nbrBuffer[ctr] = data.tangents.size() * 4 * sizeofFloat;
				if (data.tangents.size() > maxSize)
					maxSize = data.tangents.size();
				break;
			case BiTangents:
				buffer[ctr] = &data.biTangents[0].x;
				nbrBuffer[ctr] = data.biTangents.size() * 4 * sizeofFloat;
				if (data.biTangents.size() > maxSize)
					maxSize = data.biTangents.size();
				break;
			case Uvs:
				buffer[ctr] = &data.uvs[0][0].x;
				nbrBuffer[ctr] = data.uvs[0].size() * 2 * sizeofFloat;
				if (data.uvs[0].size() > maxSize)
					maxSize = data.uvs[0].size();
				break;
			case Weights:
				buffer[ctr] = &data.weights[0].x;
				nbrBuffer[ctr] = data.weights.size() * 4 * sizeofFloat;
				if (data.weights.size() > maxSize)
					maxSize = data.weights.size();
				break;
			case BoneIndices:
				buffer[ctr] = &data.boneIndices[0].x;
				nbrBuffer[ctr] = data.boneIndices.size() * 4 * sizeofFloat;
				if (data.boneIndices.size() > maxSize)
					maxSize = data.boneIndices.size();
				break;
			case Colors:
				buffer[ctr] = &data.colors[0].x;
				nbrBuffer[ctr] = data.colors.size() * 4 * sizeofFloat;
				if (data.colors.size() > maxSize)
					maxSize = data.colors.size();
				break;
			default:
				break;
			}
			++ctr;
		}

		//geometryManager.createSphereSimpleForm();
		//mesh.vertices = geometryManager.getSimpleFormGeo(gl::SimpleForm::SPHERE);
		//mesh.indices = geometryManager.getSimpleFormId(gl::SimpleForm::SPHERE);
		mesh.vertices = geometryManager.addVertices(maxSize, uint8_t(size), nbrBuffer.data(), buffer.data());
		mesh.indices = geometryManager.addIndices(data.indices.size(), &data.indices[0]);
		mesh.bounding = data.boundingInfos;
		//		mesh.name = data.name; // TODO
		mesh.defaultMaterialIndex = data.defaultMaterialIndex;
		geometryManager.attachVerticesToVertexPool(mesh.vertices, pools.first);
		geometryManager.attachIndicesToIndexPool(mesh.indices, pools.second);
	}

	// Create pool for meshs
	void AssetsManager::createPool(const std::vector<MeshInfos> &order, const std::bitset<MeshInfos::END> &infos)
	{
		auto geometryManager = &_dependencyManager.lock()->getInstance<gl::RenderManager>()->geometryManager;
		assert(geometryManager != nullptr);

		std::size_t size = infos.count();

		GLenum *typeComponent = new GLenum[size]; // {GL_FLOAT, GL_FLOAT, GL_FLOAT};			
		uint8_t *sizeTypeComponent = new uint8_t[size]; // { sizeof(float), sizeof(float), sizeof(float) };
		uint8_t *nbrComponent = new uint8_t[size];// {4, 4, 4};

		std::size_t ctr = 0;

		for (auto &e : order)
		{
			switch (e)
			{
			case Positions:
				typeComponent[ctr] = GL_FLOAT;
				sizeTypeComponent[ctr] = sizeof(float);
				nbrComponent[ctr] = 4;
				break;
			case Normals:
				typeComponent[ctr] = GL_FLOAT;
				sizeTypeComponent[ctr] = sizeof(float);
				nbrComponent[ctr] = 4;
				break;
			case Tangents:
				typeComponent[ctr] = GL_FLOAT;
				sizeTypeComponent[ctr] = sizeof(float);
				nbrComponent[ctr] = 4;
				break;
			case BiTangents:
				typeComponent[ctr] = GL_FLOAT;
				sizeTypeComponent[ctr] = sizeof(float);
				nbrComponent[ctr] = 4;
				break;
			case Uvs:
				typeComponent[ctr] = GL_FLOAT;
				sizeTypeComponent[ctr] = sizeof(float);
				nbrComponent[ctr] = 2;
				break;
			case Weights:
				typeComponent[ctr] = GL_FLOAT;
				sizeTypeComponent[ctr] = sizeof(float);
				nbrComponent[ctr] = 4;
				break;
			case BoneIndices:
				typeComponent[ctr] = GL_FLOAT;
				sizeTypeComponent[ctr] = sizeof(float);
				nbrComponent[ctr] = 4;
				break;
			case Colors:
				typeComponent[ctr] = GL_FLOAT;
				sizeTypeComponent[ctr] = sizeof(float);
				nbrComponent[ctr] = 4;
				break;

			default:
				break;
			}
			++ctr;
		}

		auto vpKey = geometryManager->addVertexPool(uint8_t(size), typeComponent, sizeTypeComponent, nbrComponent);
		auto indKey = geometryManager->addIndexPool();

		geometryManager->attachIndexPoolToVertexPool(vpKey, indKey);

		_pools.insert(std::make_pair(infos, std::make_pair(vpKey, indKey)));

		delete[] typeComponent;
		delete[] sizeTypeComponent;
		delete[] nbrComponent;
	}
}