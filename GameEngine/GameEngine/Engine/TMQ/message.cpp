#include "message.hpp"

using namespace TMQ;

std::size_t MessageBase::__sharedIdCounter = 0;

MessageBase::MessageBase(std::size_t _uid, std::size_t _tid)
	: uid(_uid)
	, tid(_tid)
{
}

MessageBase::~MessageBase()
{}