#ifndef DATA_HH_
# define DATA_HH_

# include <iostream>
# include <stdint.h>
# include <array>

class Data
{
private:
	std::size_t _sizeBuffer;
	void *_buffer;
public:
	Data();
	Data(std::size_t sizeBuffer, void *buffer);
	Data(Data const &copy);
	~Data();
	Data &operator=(Data const &data);
	std::size_t getSizeBuffer() const;
	void const * const getBuffer() const;
	bool operator==(Data const &data) const;
	bool operator!=(Data const &data) const;
	Data &operator()(std::size_t sizeBuffer, void *buffer);
};

#endif /*DATA_HH_*/