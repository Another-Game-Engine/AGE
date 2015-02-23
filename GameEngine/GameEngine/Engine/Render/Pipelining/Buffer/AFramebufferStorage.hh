#pragma once

# include <Render/Pipelining/Buffer/IFramebufferStorage.hh>

namespace AGE
{

	class AFramebufferStorage : public IFramebufferStorage
	{
	public:
		virtual ~AFramebufferStorage();

	protected:
		AFramebufferStorage();
	};

}