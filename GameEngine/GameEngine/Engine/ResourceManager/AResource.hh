#ifndef		ARESOURCE_HH_
#define		ARESOURCE_HH_

#include <string>

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

public:
	AResource(void);
	virtual ~AResource(void);

	virtual	bool	load(std::string const &path) = 0;

	bool			isLoaded() const;
	void			setLoaded(bool loaded);

	ResourceType	getType() const;
};

}

#endif