
#ifndef SINGLETON_HH_
#define SINGLETON_HH_

template<class T>
class Singleton
{
private:
  static T    *_instance;
  Singleton();
  ~Singleton();

public:
	static void 		freeMemory();
  	static T           *instance();

};

#include "Singleton.hpp"

#endif