#pragma once

#include <Core/AScene.hh>
#include <Components/RotationForce.hpp>
#include <Systems/RotationForceSystem.hpp>

class BenchmarkScene : public AScene	
{
public:
	BenchmarkScene(std::weak_ptr<Engine> engine)
		: AScene(engine)
	{}

	virtual ~BenchmarkScene(void)
	{}

	virtual bool 			userStart()
	{
		addSystem<RotationForceSystem>(0);
		_logFile.open("LogFile.txt", std::ios::app);
		return true;
	}

	virtual bool 			userUpdate(double time)
	{
		++_frameCounter;
		++_chunkFrame;
		_timeCounter += time;
		_chunkCounter += time;


		if (_collection.size() > 4)
		{
			for (auto i = 0; i < 4; ++i)
			{
				destroy(_collection.front());
				_collection.remove(_collection.front());
			}
		}
		for (auto i = 0; i < 8; ++i)
		{
			auto e = createEntity();
			if (i % 2)
			{
				e->addComponent<Component::RotationForce>(glm::vec3(2, 0, 4));
			}
			_collection.push_back(e);
		}

		if (_chunkCounter >= _maxChunk)
		{
			_logFile << _chunkFrame << ", ";
			_chunkCounter = 0.0;
			_chunkFrame = 0;
		}
		if (_timeCounter >= _maxTime)
		{
			_logFile << std::endl << "Total frames : " << _frameCounter << " -- Entity created : " << _collection.size() << std::endl << "----------------" << std::endl;
			_logFile.close();
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
	std::list<Entity> _collection;
};