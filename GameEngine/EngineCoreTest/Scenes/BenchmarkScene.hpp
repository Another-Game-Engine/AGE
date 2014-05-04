#pragma once

#include <Core/AScene.hh>

#include <Systems/LifetimeSystem.hpp>
#include <Systems/BulletDynamicSystem.hpp>
#include <Systems/CollisionAdderSystem.hpp>
#include <Systems/CollisionCleanerSystem.hpp>

class BenchmarkScene : public AScene	
{
public:
	BenchmarkScene(std::weak_ptr<Engine> &&engine)
		: AScene(std::move(engine))
	{}

	virtual ~BenchmarkScene(void)
	{}

	virtual bool 			userStart()
	{

		rct<Component::Lifetime>();

		addSystem<BulletDynamicSystem>(0);
		addSystem<CollisionAdder>(1);
		addSystem<LifetimeSystem>(2);
		addSystem<CollisionCleaner>(3);

		srand(42);

		_logFile.open("LogFile.log", std::ios::app);

		return true;
	}

	virtual bool 			userUpdate(double time)
	{
		++_frameCounter;
		++_chunkFrame;
		_timeCounter += time;
		_chunkCounter += time;

		if (_chunkCounter >= _maxChunk)
		{
			for (auto i = 0; i < 14; ++i)
			{
				auto e = createEntity();
				addComponent<Component::Lifetime>(e);
				auto rb = addComponent<Component::RigidBody>(e, e, 1.0f);
				rb->setCollisionShape(Component::RigidBody::CollisionShape::SPHERE);
				e->setLocalTransform(glm::translate(e->getLocalTransform(), glm::vec3((rand() % 20) - 10, (rand() % 20) - 5, (rand() % 20) - 10)));
			}
			_logFile << _chunkFrame << ", ";
			_chunkCounter = 0.0;
			_chunkFrame = 0;
		}
		if (_timeCounter >= _maxTime)
		{
			_logFile << std::endl << "Total frames : " << _frameCounter << " -- Entity created : " << this->getNumberOfEntities() << std::endl << "----------------" << std::endl;
			_logFile.close();

			std::ofstream saveFile("SaveFile.save", std::ios::binary);
			save<cereal::PortableBinaryOutputArchive>(saveFile);
			return false;
		}
		return true;
	}
private:
	std::size_t _frameCounter = 0;
	double _timeCounter = 0.0;
	double _maxTime = 8.0;
	double _chunkCounter = 0.0;
	double _maxChunk = 0.25;
	std::size_t _chunkFrame = 0;
	std::ofstream _logFile;
};