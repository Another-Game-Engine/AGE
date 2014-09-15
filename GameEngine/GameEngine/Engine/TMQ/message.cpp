#include "message.hpp"

using namespace TMQ;

MessageBase::MessageBase(std::size_t _uid)
	: uid(_uid)
{
}

MessageBase::~MessageBase()
{}