#ifndef ATTRIBUTE_HH_
# define ATTRIBUTE_HH_

# include "Utils/OpenGL.hh"
# include <stdint.h>

class Attribute
{
public:
	Attribute(GLenum type, uint8_t sizeType, uint8_t nbrComponent);
	~Attribute();
	Attribute(Attribute const &copy);
	Attribute &operator=(Attribute const &attribute);
	GLenum getType() const;
	uint8_t getSizeType() const;
	uint8_t getNbrComponent() const;
private:
	GLenum _type;
	uint8_t _sizeType;
	uint8_t _nbrComponent;
};

#endif /*!ATTRIBUTE*/