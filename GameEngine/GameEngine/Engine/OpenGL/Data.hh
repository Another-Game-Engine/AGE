#ifndef DATA_HH_
# define DATA_HH_

# include <iostream>
# include <stdint.h>
# include <array>

class Data
{
private:
	uint32_t _nbrByte;
	uint32_t _nbrComponent;
	uint32_t _nbrElement;
	uint32_t _sizeBuffer;
	void *_buffer;
public:
	Data(uint32_t nbrByte, uint32_t nbrComponent, uint32_t nbrBuffer, void *buffer);
	Data(Data const &copy);
	~Data();
	Data &operator=(Data const &data);
	uint32_t getNbrComponent() const;
	uint32_t getNbrElement() const;
	uint32_t getNbrByte() const;
	uint32_t getSizeBuffer() const;
	bool consistency(Data const &data) const;
	void const * const getBuffer() const;
	bool operator==(Data const &data) const;
	bool operator!=(Data const &data) const;
	void clearBuffer();
};

#endif /*DATA_HH_*/