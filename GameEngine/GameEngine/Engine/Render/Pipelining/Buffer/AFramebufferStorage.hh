#pragma once

# include <Render/Pipelining/Buffer/IFramebufferStorage.hh>

class AFramebufferStorage : public IFramebufferStorage
{
public:
	virtual ~AFramebufferStorage();

protected:
	AFramebufferStorage();
};