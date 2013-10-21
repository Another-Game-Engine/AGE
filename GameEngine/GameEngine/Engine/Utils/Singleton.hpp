#ifdef  SINGLETON_HH_

template<class T, class Param>
T 	*Singleton<T, Param>::_instance = NULL;

template<class T, class Param>
void 		Singleton<T, Param>::freeMemory()
{
	delete _instance;
}

template<class T, class Param>
T           *Singleton<T, Param>::instance()
{
  if (_instance == NULL)
  {
    _instance = new T(new Param);
//	std::atexit(freeMemory);
  }
  return (_instance);
}

#endif