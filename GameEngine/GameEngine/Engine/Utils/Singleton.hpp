#ifdef  SINGLETON_HH_

template<class T>
T 	*Singleton<T>::_instance = nullptr;

template<class T>
std::once_flag 	Singleton<T>::_flag;

template<class T>
void 		Singleton<T>::freeMemory()
{
	if (_instance)
		delete _instance;
	_instance = nullptr;
}

template<class T>
T           *Singleton<T>::getInstance()
{
  if (_instance == NULL)
  {
    _instance = new T();
	std::call_once(_flag, []()
	{
		std::atexit(&Singleton<T>::freeMemory);
	});
  }
  return (_instance);
}

#endif