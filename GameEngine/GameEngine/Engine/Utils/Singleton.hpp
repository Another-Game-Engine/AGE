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
	assert(_instance != nullptr);
	return (_instance);
}

template<class T>
void           Singleton<T>::setInstance()
{
	std::call_once(_flag, []()
	{
		Singleton<T>::_instance = new T();
		std::atexit(&Singleton<T>::freeMemory);
	});
}

#endif