#pragma once

#include <Core/AScene.hh>
#include <fstream>

class Engine;

class BenchmarkScene : public AScene	
{
public:
	BenchmarkScene(std::weak_ptr<Engine> &&engine);
	virtual ~BenchmarkScene();

	virtual bool userStart();
	virtual bool userUpdate(double time);

private:
	std::size_t _frameCounter = 0;
	double _timeCounter = 0.0;
	double _maxTime = 8.0;
	double _chunkCounter = 0.0;
	double _maxChunk = 0.25;
	std::size_t _chunkFrame = 0;
	std::ofstream _logFile;
};