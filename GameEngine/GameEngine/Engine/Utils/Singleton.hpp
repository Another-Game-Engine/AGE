#ifdef  SINGLETON_HH_

template<class T>
T 	*Singleton<T>::_instance = NULL;

template<class T>
void 		Singleton<T>::freeMemory()
{
	delete _instance;
}

template<class T>
T           *Singleton<T>::instance()
{
  if (_instance == NULL)
  {
    _instance = new T();
//	std::atexit(freeMemory);
  }
  return (_instance);
}

#endif