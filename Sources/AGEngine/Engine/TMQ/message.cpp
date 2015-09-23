#include "message.hpp"

using namespace TMQ;

std::size_t MessageBase::__sharedIdCounter = 0;

MessageBase::MessageBase(std::size_t _uid, std::size_t _tid)
	: uid(_uid)
	, tid(_tid)
{
}

MessageBase::~MessageBase()
{
}

MessageBase::MessageBase(const MessageBase&o)
{
	uid = o.uid;
	tid = o.tid;
	_used = o._used;
}

MessageBase &MessageBase::operator=(const MessageBase&o)
{
	uid = o.uid;
	tid = o.tid;
	_used = o._used;
	return *this;
}

MessageBase::MessageBase(MessageBase&&o)
{
	uid = o.uid;
	tid = o.tid;
	_used = o._used;
}

MessageBase &MessageBase::operator=(MessageBase&&o)
{
	uid = o.uid;
	tid = o.tid;
	_used = o._used;
	return *this;
}

MessageBase *MessageBase::clone(char* dest)
{
	return _clone(dest);
}