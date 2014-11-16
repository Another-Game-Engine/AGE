#pragma once

# include <vector>
# include <memory>
# include <Render/Data.hh>

class Data;

class Vertices
{
public:
	Vertices(std::vector<Data> const &data);
	Vertices(std::vector<Data> &&data);

private:
	std::vector<std::shared_ptr<Data>> _data;
};