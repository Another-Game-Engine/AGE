#pragma once

#include <Core/AScene.hh>
#include <Systems/BullshitSystem.hpp>

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
		addSystem<BullshitSystem>(0);
		_logFile.open("LogFile.txt", std::ios::app);
		return true;
	}

	virtual bool 			userUpdate(double time)
	{
		++_frameCounter;
		++_chunkFrame;
		_timeCounter += time;
		_chunkCounter += time;

		for (auto i = 0; i < 8; ++i)
		{
			auto e = createEntity();
			if (i % 2)
			{
				e->addComponent<Component::Bullshit>(0.3f);
			}
		}

		if (_chunkCounter >= _maxChunk)
		{
			_logFile << _chunkFrame << ", ";
			_chunkCounter = 0.0;
			_chunkFrame = 0;
		}
		if (_timeCounter >= _maxTime)
		{
			_logFile << std::endl << "Total frames : " << _frameCounter << " -- Entity created : " << this->getNumberOfEntities() << std::endl << "----------------" << std::endl;
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
};