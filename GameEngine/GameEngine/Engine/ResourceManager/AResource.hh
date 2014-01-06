#ifndef		ARESOURCE_HH_
#define		ARESOURCE_HH_

#include <string>

class Engine;

namespace	Resources
{

class AResource
{
public:
	enum	ResourceType
	{
		SHAREDMESH,
		TEXTURE
	};

private:
	bool			_isLoaded;
	ResourceType	_type;
protected:
	Engine          *_engine;
public:
	AResource(void);
	virtual ~AResource(void);

	virtual	bool	load(std::string const &path) = 0;

	bool			isLoaded() const;
	void			setLoaded(bool loaded);
	inline void     setEngine(Engine &engine){ _engine = &engine; }
	inline Engine   *getEngine() { return _engine; }
	ResourceType	getType() const;
};

}

#endif