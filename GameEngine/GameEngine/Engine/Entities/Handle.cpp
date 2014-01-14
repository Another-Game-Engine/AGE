#include <Entities\Handle.hh>
#include <Entities\EntityData.hh>
#include <Core/Engine.hh>
#include <Components/Component.hh>
#include <limits>
#include <Core/AScene.hh>

Handle::Handle(unsigned int id, AScene *manager)
: _id(id), _manager(manager)
{}

Handle::~Handle()
{}


const unsigned int Handle::getId() const
{
	return _id;
}

EntityData *Handle::operator->()
{
	return _manager->get(*this);
}

EntityData *Handle::get() const
{
	static unsigned int max = std::numeric_limits<unsigned int>::max();
	if (!_manager || _id == max)
		return nullptr;
	return _manager->get(*this);
}

bool Handle::operator<(const Handle &o) const
{
	return _id < o._id;
}

bool Handle::operator==(const Handle &o) const
{
	return _id == o._id;
}

Handle::Handle(const Handle &o)
{
	_id = o._id;
	_manager = o._manager;
}

Handle &Handle::operator=(const Handle &o)
{
	_id = o._id;
	_manager = o._manager;
	return *this;
}