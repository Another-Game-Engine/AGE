#pragma once

#include <Render/Key.hh>
#include <Render/ProgramResources/IProgramResources.hh>

class UniformBuffer;
class BlockResources;

class IInterfaceBlock
{
public:
	virtual IInterfaceBlock &update() = 0;
	virtual UniformBuffer const &buffer() const = 0;
	virtual BlockResources *get_resource(std::string const &name) = 0;
	virtual Key<ProgramResource> get_key(std::string const &name) = 0;
	virtual BlockResources *get_resource(Key<ProgramResource> const &key) = 0;

protected:
	IInterfaceBlock(){};
	virtual ~IInterfaceBlock(){};
};