#pragma once

class IBlockResources
{
public:
	virtual bool safe(size_t size) const = 0;
};