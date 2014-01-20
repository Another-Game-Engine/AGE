#ifndef SMARTPOINTER_HH_
#define SMARTPOINTER_HH_

#include <assert.h>
#include <stdlib.h>

#include "Utils.hh"

template<class T>
class SmartPointer
{
public:
	struct SReferenceCount
	{
	 	T 									*pointer;
 		unsigned long int 	count;
	};

private:
	SReferenceCount 			*_reference;
		
public:
	INLINE SmartPointer();
	INLINE SmartPointer(SmartPointer const &oth);
	INLINE SmartPointer(T *ptr);
	INLINE ~SmartPointer();

	INLINE void 					SetReferencePointer(SReferenceCount *ref);

  template<class U>
  INLINE operator SmartPointer<U>() const;

	INLINE SmartPointer 	&operator=(SmartPointer const &oth);
	INLINE SmartPointer 	&operator=(T *ptr);
	INLINE T 				&operator*() const;
	INLINE T 				*operator->() const;
	INLINE bool 			operator==(SmartPointer const &oth) const;
	INLINE bool 			operator!=(SmartPointer const &oth) const;
	INLINE bool 			operator==(T *oth) const;
	INLINE bool 			operator!=(T *oth) const;
	INLINE T                *get() const;
	INLINE void             reset(T* newValue = nullptr);
	INLINE bool             operator<(SmartPointer const &oth) const;
};

#include "SmartPointer.hpp"

#endif