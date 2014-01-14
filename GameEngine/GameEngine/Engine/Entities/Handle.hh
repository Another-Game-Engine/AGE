#ifndef   __HANDLE_HH__
# define  __HANDLE_HH__

#include <Utils/MetaData.hpp>

class EntityData;
class AScene;

class Handle
{
public:
	META_DATA(Handle);
	Handle(unsigned int id = 0, AScene *manager = nullptr);
	~Handle();
	const unsigned int getId() const;
	EntityData *operator->();
	EntityData *get() const;
	bool operator<(const Handle &o) const;
	bool operator==(const Handle &o) const;
	Handle(const Handle &o);
	Handle &operator=(const Handle &o);
private:
	unsigned int _id;
	AScene *_manager;
};

#endif    //__HANDLE_HH__