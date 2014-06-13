#include <string>
#include <fstream>
#include <algorithm>
#include "PerformanceDebugger.hh"

static const char *DEV_NAME = "Anthony";

PerformanceDebugger::PerformanceDebugger()
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
		_counters[counterName] = Counter(average, logFileName);
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
		return ;
	it->second.start = std::chrono::high_resolution_clock::now();
}


void PerformanceDebugger::stop(std::string const &counterName)
{
	auto &time = std::chrono::high_resolution_clock::now();
	auto &it = _counters.find(counterName);
	if (it == _counters.end())
		return ;
	it->second.stop = time;
}

void PerformanceDebugger::changeFile(std::string const &logFileName, std::string const &counterName)
{
	auto &it = _counters.find(counterName);
	if (it == _counters.end())
		return ;
	it->second.file = logFileName;
}

bool PerformanceDebugger::logNow(std::string const &counterName, std::string const &comment)
{
	auto &time = std::chrono::high_resolution_clock::now();
	auto &it = _counters.find(counterName);
	if (it == _counters.end())
		return false;
	std::ofstream file(it->second.file, std::ios::app);
	if (file.is_open())
	{
		if (it->second.start < it->second.stop)
		{
			file << '['
				 << it->first
				 << "] ("
				 << DEV_NAME
				 << ") now -> "
				 << (it->second.stop - it->second.start).count()
				 << " ms : "
				 << comment
				 << std::endl;			
		}
		else
		{
			file << '['
				<< it->first
				<< "] ("
				<< DEV_NAME
				<< ") now -> "
				<< (time - it->second.start).count()
				<< " ms : "
				<< comment
				<< std::endl;
		}
		file.close();
		return true;
	}
	return false;
}

bool PerformanceDebugger::logAverage(std::string const &counterName, std::string const &comment)
{
	auto &it = _counters.find(counterName);
	if (it == _counters.end())
		return false;
	++it->second.nbIter;
	if (it->second.nbIter < it->second.averageTime)
	{
		auto now = std::chrono::high_resolution_clock::now();
		it->second.average[it->second.nbIter] = now - it->second.prev;
		it->second.prev = now;
		return false;
	}
	std::ofstream file(it->second.file, std::ios::app);
	if (file.is_open())
	{
		auto &end = _counters.end();
		std::chrono::duration<float> sum;
		for (int i = 0; i < it->second.averageTime; ++i)
			sum += it->second.average[i];
		file << '['
			 << it->first
			 << "] ("
			 << DEV_NAME
			 << ") average on "
			 << it->second.averageTime
			 << " -> "
			 << (sum / it->second.averageTime).count()
			 << " ms : "
			 << comment
			 << std::endl;
		file.close();
		it->second.nbIter = 0;
		return true;
	}
	return false;
}

PerformanceDebugger::Counter::Counter(int averageCounter, std::string const &file)
 : averageTime(averageCounter),
 nbIter(0),
 average(new std::chrono::duration<float>[averageCounter]),
 prev(std::chrono::high_resolution_clock::now()),
 start(std::chrono::high_resolution_clock::now()),
 stop(std::chrono::high_resolution_clock::now())
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
		file = counter.file;
	}
	return *this;
}

PerformanceDebugger::Counter::~Counter()
{
	delete[] average;
}