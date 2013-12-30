#ifndef   __HANDLE_HH__
# define  __HANDLE_HH__

class Entity;
class AScene;

class Handle
{
public:
	Handle(unsigned int id = 0, AScene *manager = nullptr);
	~Handle();
	const unsigned int getId() const;
	Entity *operator->();
	Entity *get() const;
	bool operator<(const Handle &o) const;
	bool operator==(const Handle &o) const;
	Handle(const Handle &o);
	Handle &operator=(const Handle &o);
private:
	unsigned int _id;
	AScene *_manager;
};

#endif    //__HANDLE_HH__