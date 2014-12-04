#pragma once

class IBlockResources
{
public:
	virtual bool safe(size_t size) const = 0;
	virtual size_t offset() const = 0;
	virtual IBlockResources &offset(size_t o) = 0;
	virtual size_t size_array() const = 0;
	virtual IBlockResources &size_array(size_t sa) = 0;
	virtual size_t stride() const = 0;
	virtual IBlockResources &stride(size_t s) = 0;
	virtual size_t size() const = 0;
	virtual void const *data() const = 0;
};