#ifdef 	SMARTPOINTER_HH_

template<class T>
INLINE SmartPointer<T>::SmartPointer() :
	_reference(NULL)
{
}

template<class T>
INLINE SmartPointer<T>::SmartPointer(SmartPointer const &oth) :
	_reference(NULL)
{
	*this = oth;
}

template<class T>
INLINE SmartPointer<T>::SmartPointer(T *ptr) :
	_reference(NULL)
{
	*this = ptr;
}

template<class T>
INLINE SmartPointer<T>::~SmartPointer()
{
	if (_reference)
	{
		_reference->count -= 1;
		if (_reference->count == 0)
		{
			delete _reference->pointer;
			delete _reference;
		}
	}
}

template<class T>
INLINE void 					SmartPointer<T>::SetReferencePointer(SReferenceCount *ref)
{	
	if (_reference)
	{
		_reference->count -= 1;
		if (_reference->count == 0)
		{
			delete _reference->pointer;
			delete _reference;
		}
	}
	_reference = ref;
	if (_reference)
		_reference->count += 1;
}

template<class T>
template<class U>
INLINE SmartPointer<T>::operator SmartPointer<U>() const
{
	SmartPointer<U> 							casted(NULL);

	casted.SetReferencePointer(reinterpret_cast<typename SmartPointer<U>::SReferenceCount*>(_reference));
	return (casted);
}

template<class T>
INLINE SmartPointer<T> 	&SmartPointer<T>::operator=(SmartPointer<T> const &oth)
{
	if (&oth != this)
	{
		if (_reference)
		{
			_reference->count -= 1;
			if (_reference->count == 0)
			{
				delete _reference->pointer;
				delete _reference;
			}
		}
		_reference = oth._reference;
		if (_reference)
			_reference->count += 1;
	}
	return (*this);
}

template<class T>
INLINE SmartPointer<T> 	&SmartPointer<T>::operator=(T *ptr)
{
	if (_reference)
	{
		_reference->count -= 1;
		if (_reference->count == 0)
		{
			delete _reference->pointer;
			delete _reference;
		}
	}
	if (ptr)
	{
		_reference = new SReferenceCount;
		_reference->pointer = ptr;
		_reference->count = 1;
	}
	else
		_reference = NULL;
	return (*this);
}

template<class T>
INLINE T 				&SmartPointer<T>::operator*() const
{
	assert(_reference != NULL);
	return (*(_reference->pointer));
}

template<class T>
INLINE T 				*SmartPointer<T>::operator->() const
{
	assert(_reference != NULL);
	return (_reference->pointer);
}

template<class T>
INLINE bool 			SmartPointer<T>::operator==(SmartPointer const &oth) const
{
	return (_reference == oth._reference);	
}

template<class T>
INLINE bool 			SmartPointer<T>::operator!=(SmartPointer const &oth) const
{
	return (_reference != oth._reference);	
}

template<class T>
INLINE bool 			SmartPointer<T>::operator==(T *oth) const
{
	if (_reference == NULL)
	{
		if (oth == NULL)
			return (true);
		return (false);		
	}
	return (_reference->pointer == oth);	
}

template<class T>
INLINE bool 			SmartPointer<T>::operator!=(T *oth) const
{
	if (_reference == NULL)
	{
		if (oth == NULL)
			return (false);
		return (true);		
	}
	return (_reference->pointer != oth);	
}

#endif