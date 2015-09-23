#include <map>
#include <Utils/Containers/Vector.hpp>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "PerformanceDebugger.hh"

PerformanceDebugger::PerformanceDebugger(const std::string &developperName /* = "UNKNOWN DEVELOPPER" */)
: _developper(developperName)
{
}

PerformanceDebugger::~PerformanceDebugger()
{
}

PerformanceDebugger *PerformanceDebugger::setCounter(std::string const &logFileName,
													 std::string const &counterName,
												     int average)
{
	auto &it = _counters.find(counterName);
	if (it == _counters.end())
		_counters.insert(std::pair<std::string const &, Counter>(counterName, Counter(average, logFileName)));
	else
		it->second = Counter(average, logFileName);
	return this;
}

void PerformanceDebugger::delCounter(std::string const &counterName)
{
	_counters.erase(counterName);
}

void PerformanceDebugger::start(std::string const &counterName)
{
	auto &it = _counters.find(counterName);
	if (it == _counters.end())
	{
		std::cerr << "Error counter " << counterName << " does not exist.";
		return;
	}
	it->second.start = std::chrono::high_resolution_clock::now();
}


void PerformanceDebugger::stop(std::string const &counterName)
{
	auto &time = std::chrono::high_resolution_clock::now();
	auto &it = _counters.find(counterName);
	if (it == _counters.end())
	{
		std::cerr << "Error counter " << counterName << " does not exist.";
		return;
	}
	it->second.stop = time;
}

void PerformanceDebugger::changeFile(std::string const &logFileName, std::string const &counterName)
{
	auto &it = _counters.find(counterName);
	if (it == _counters.end())
	{
		std::cerr << "Error counter " << counterName << " does not exist.";
		return;
	}
	it->second.fileName = logFileName;
	if (it->second.file.is_open())
		it->second.file.close();
	it->second.file = std::ofstream(logFileName, std::ios::app);
}

bool PerformanceDebugger::logNow(std::string const &counterName, std::string const &comment)
{
	auto &time = std::chrono::high_resolution_clock::now();
	auto &it = _counters.find(counterName);
	if (it == _counters.end())
	{
		std::cerr << "Error counter " << counterName << " does not exist.";
		return false;
	}
	if (it->second.file.is_open())
	{
		if (it->second.start < it->second.stop)
		{
			it->second.file << '['
							<< it->first
							<< "] ("
							<< _developper
							<< ") now -> "
							<< (it->second.stop - it->second.start).count() / 1000
							<< " ms : "
							<< comment
							<< std::endl;
		}
		else
		{
			it->second.file << '['
							<< it->first
							<< "] ("
							<< _developper
							<< ") now -> "
							<< (time - it->second.start).count() / 1000
							<< " ms : "
							<< comment
							<< std::endl;
		}
		return true;
	}
	return false;
}

bool PerformanceDebugger::logAverage(std::string const &counterName, std::string const &comment)
{
	auto &it = _counters.find(counterName);
	if (it == _counters.end())
	{
		std::cerr << "Error counter " << counterName << " does not exist.";
		return false;
	}
	++it->second.nbIter;
	if (it->second.nbIter < it->second.averageTime)
	{
		auto now = std::chrono::high_resolution_clock::now();
		it->second.average[it->second.nbIter] = now - it->second.prev;
		it->second.prev = now;
		return false;
	}
	if (it->second.file.is_open())
	{
		auto &end = _counters.end();
		std::chrono::duration<float> sum;
		for (int i = 0; i < it->second.averageTime; ++i)
			sum += it->second.average[i];
		it->second.file << '['
						<< it->first
						<< "] ("
						<< _developper
						<< ") average on "
						<< it->second.averageTime
						<< " -> "
						<< (float)((sum / (float)it->second.averageTime).count()) * 1000.f
						<< " ms : "
						<< comment
						<< std::endl;
		it->second.nbIter = 0;
		return true;
	}
	return false;
}

PerformanceDebugger::Counter::Counter(int averageCounter, std::string const &File)
	: averageTime(averageCounter),
	nbIter(0),
	average(averageCounter),
	prev(std::chrono::high_resolution_clock::now()),
	start(std::chrono::high_resolution_clock::now()),
	stop(std::chrono::high_resolution_clock::now()),
	fileName(File),
	file(File, std::ios::app)
{
		if (!file.is_open())
			std::cerr << "Cannot open " << File << std::endl;
}

PerformanceDebugger::Counter::Counter(const Counter &counter)
	: averageTime(counter.averageTime),
	nbIter(counter.nbIter),
	average(counter.average),
	prev(counter.prev),
	start(counter.start),
	stop(counter.stop),
	fileName(counter.fileName),
	file(counter.fileName, std::ios::app)
{
}

PerformanceDebugger::Counter &
	PerformanceDebugger::Counter::operator=(PerformanceDebugger::Counter const &counter)
{
		if (this != &counter)
		{
			averageTime = counter.averageTime;
			nbIter = counter.nbIter;
			for (int i = 0; i < averageTime; ++i)
				average[i] = counter.average[i];
			prev = counter.prev;
			start = counter.start;
			stop = counter.stop;
			fileName = counter.fileName;
			file = std::ofstream(fileName, std::ios::app);
		}
		return *this;
}

PerformanceDebugger::Counter::~Counter()
{
	if (file.is_open())
		file.close();
}

void PerformanceDebugger::setDevelopperName(const std::string &developperName)
{
	_developper = developperName;
}