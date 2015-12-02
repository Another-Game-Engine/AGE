
#ifndef SINGLETON_HH_
#define SINGLETON_HH_

#include <mutex>
#include <Utils/Visibility.hpp>

template<class T>
class __declspec(dllexport) Singleton
{
private:
  static T    *_instance;
  static std::once_flag _flag;
protected:
  Singleton();
  ~Singleton();
  static void freeMemory();
public:
  	static T           *getInstance();
  	static void        setInstance();
};

#include "Singleton.hpp"

#endif