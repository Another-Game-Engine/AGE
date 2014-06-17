#pragma once

#include <chrono>
#include "Utils/Dependency.hpp"

class string;
template <class T1, class T2> struct pair;
template <class T, class Alloc = allocator<T>> class vector;
template <class Key, class T, class Compare = less<Key>, class Alloc = allocator<pair<const Key, T>>> class map;
class PerformanceDebugger : public Dependency
{
public:
	// CTOR
	PerformanceDebugger();
	// DTOR
	virtual ~PerformanceDebugger();

	// Delete coplien's form
	PerformanceDebugger(const PerformanceDebugger &) = delete;
	PerformanceDebugger & operator=(const PerformanceDebugger &) = delete;

	// Add a counter to the list of available counter
	// logFileName : name of the file use to store the counter
	// name : name of the counter
	// return himself
	PerformanceDebugger *setCounter(std::string const &logFileName, std::string const &counterName, int average = 1);
	// Delete a counter from the list
	// Do nothing if the counter does not exist
	// counterName : name of the counter
	void delCounter(std::string const &counterName);
	// Start the counter pointed by the name
	// Do nothing if the counter does not exist
	// Restart the counter if the counter already exist
	// counterName : name of the counter
	void start(std::string const &counterName);
	// Stop the counter pointed by the name
	// Do nothing if the counter does not exist
	// Return time since setCounter if counter was not started
	// counterName : name of the counter
	void stop(std::string const &counterName);
	// Change the destination file of the log
	// Do nothing if the counter does not exist
	// Does not transfered old previous data logged
	// logFileName : name of the file use to store the counter
	// counterName : name of the counter
	void changeFile(std::string const &logFileName, std::string const &counterName);
	// log the difference between the previous start and stop
	// Do nothing if the counter does not exist
	// counterName : name of the counter
	// comment : whatever you wanna say
	bool logNow(std::string const &counterName, std::string const &comment = "");
	// log the time's difference between 2 calls
	// log automatically when average time is reach
	// counterName : name of the counter
	// comment : whatever you wanna say
	bool logAverage(std::string const &counterName, std::string const &comment = "");

private:
	struct Counter
	{
		int averageTime;
		int nbIter;
		std::vector<std::chrono::duration<float>> average;
		std::chrono::high_resolution_clock::time_point prev;
		std::chrono::high_resolution_clock::time_point start;
		std::chrono::high_resolution_clock::time_point stop;
		std::string fileName;
		std::ofstream file;
		Counter(int averageCounter = 1, std::string const &file = "default.log");
		Counter(const Counter &counter);
		Counter &operator=(Counter const &counter);
		~Counter();
	};

	std::map<std::string, Counter> _counters;
};
