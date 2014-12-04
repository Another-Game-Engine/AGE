#pragma once

# include <Render/ProgramResources/IBlockResources.hh>

class ABlockResources : public IBlockResources
{
public:
	ABlockResources();
	ABlockResources(ABlockResources const &copy);

public:
	virtual size_t offset() const override final;
	virtual IBlockResources &offset(size_t o) override final;
	virtual size_t size_array() const override final;
	virtual IBlockResources &size_array(size_t sa) override final;
	virtual size_t stride() const override final;
	virtual IBlockResources &stride(size_t s) override final;

private:
	size_t _offset;
	size_t _size_array;
	size_t _stride;
};