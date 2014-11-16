#pragma once

# include <vector>
# include <Render/Data.hh>

class Vertices
{
public:
	Vertices(std::vector<Data> const &data);
	Vertices(std::vector<Data> &&data);

private:
	std::vector<Data> _data;
};