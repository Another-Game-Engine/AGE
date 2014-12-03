
#ifndef SINGLETON_HH_
#define SINGLETON_HH_

#include <mutex>

template<class T>
class Singleton
{
private:
  static T    *_instance;
  static std::once_flag _flag;
protected:
  Singleton();
  virtual ~Singleton();
  static void 		freeMemory();
public:
  	static T           *getInstance();

};

#include "Singleton.hpp"

#endif