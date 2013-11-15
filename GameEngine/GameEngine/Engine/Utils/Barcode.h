#ifndef		__BARCODE_H__
# define	__BARCODE_H__

#include   <bitset>
#include   <assert.h>

#include "Core/Settings.h"

namespace   Component
{
	struct  Base;
};

class	Entity;

class Barcode
{
public:

	Barcode();
	Barcode(const ::Entity &entity);
	Barcode(const Barcode &other);
	Barcode &operator=(const Barcode &other);
	~Barcode();
	bool match(const std::bitset<COMPONENTS_MAX_NUMBER> &set) const;
	bool match(const ::Entity &entity) const;
	bool match(const Barcode &entity) const;
	void reset();
	void add(unsigned int componentId);
	void remove(unsigned int componentId);
	bool isEmpty() const;
	bool isSet(unsigned int componentId) const;

	template <class T>
	void add()
	{
		applyChange(T::getTypeId(), true);
	}

	template <class T>
	void remove()
	{
		applyChange(T::getTypeId(), false);
	}

	template <class T>
	bool isSet() const
	{
		return code_.test(T::getTypeId());
	}

private:
	void applyChange(unsigned int componentId, bool tof)
	{
		assert(componentId < COMPONENTS_MAX_NUMBER, "Component ID > COMPONENTS_MAX_NUMBER");
		code_[componentId] = tof;
	}

	std::bitset<COMPONENTS_MAX_NUMBER> code_;
};

#endif		//__BARCODE_H__