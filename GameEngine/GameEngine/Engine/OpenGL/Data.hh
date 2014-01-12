#ifndef DATA_HH_
# define DATA_HH_

# include <iostream>
# include <stdint.h>
# include <array>

class Data
{
private:
	uint32_t _sizeBuffer;
	void *_buffer;
public:
	Data(uint32_t sizeBuffer, void *buffer);
	Data(Data const &copy);
	~Data();
	Data &operator=(Data const &data);
	uint32_t getSizeBuffer() const;
	void const * const getBuffer() const;
	bool operator==(Data const &data) const;
	bool operator!=(Data const &data) const;
	Data &operator()(uint32_t sizeBuffer, void *buffer);
};

#endif /*DATA_HH_*/