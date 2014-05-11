#ifndef		__BARCODE_H__
# define	__BARCODE_H__

//#include   <bitset>
#include   <boost/dynamic_bitset/dynamic_bitset.hpp>
//#include   <assert.h>

//namespace   Component
//{
//	struct  Base;
//};

class	EntityData;

class Barcode
{
public:

	Barcode();
	Barcode(EntityData &entity);
	Barcode(Barcode &&o);
	Barcode(Barcode &other);
	Barcode &operator=(const Barcode &other);
	~Barcode();
	bool match(boost::dynamic_bitset<> &set);
	bool match(EntityData &entity);
	bool match(Barcode &entity);
	void reset();
	void add(std::size_t componentId);
	void remove(std::size_t componentId);
	bool isEmpty() const;
	bool isSet(std::size_t componentId) const;

private:
	void applyChange(std::size_t componentId, bool tof)
	{
		if (code_.size() <= componentId)
			code_.resize(componentId + 1);
		code_[componentId] = tof;
	}

	boost::dynamic_bitset<> code_;
};

#endif		//__BARCODE_H__