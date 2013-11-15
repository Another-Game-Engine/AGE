#include "Barcode.h"
#include <Engine/Entities/Entity.hh>

Barcode::Barcode()
{}

Barcode::Barcode(const Entity &entity)
{
	code_ = entity.getCode().code_;
}

Barcode::Barcode(const Barcode &other)
{
	code_ = other.code_;
}

Barcode &Barcode::operator=(const Barcode &other)
{
	code_ = other.code_;
	return *this;
}

Barcode::~Barcode()
{}

bool Barcode::match(const std::bitset<COMPONENTS_MAX_NUMBER> &set) const
{
	return((set & code_) == code_);
}

bool Barcode::match(const Entity &entity) const
{
	return((entity.getCode().code_ & code_) == code_);
}

bool Barcode::match(const Barcode &other) const
{
	return((other.code_ & code_) == code_);
}

void Barcode::reset()
{
	code_.reset();
}

void Barcode::add(unsigned int componentId)
{
	applyChange(componentId, true);
}

void Barcode::remove(unsigned int componentId)
{
	applyChange(componentId, false);
}

bool Barcode::isEmpty() const
{
	return (code_.none());
}

bool Barcode::isSet(unsigned int componentId) const
{
	return code_.test(componentId);
}