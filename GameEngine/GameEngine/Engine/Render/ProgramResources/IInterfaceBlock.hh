#pragma once

#include <Render/Key.hh>
#include <Render/ProgramResources/IProgramResources.hh>

class UniformBuffer;
class BlockResources;

class IInterfaceBlock : public std::enable_shared_from_this<IInterfaceBlock>
{
public:
	virtual IInterfaceBlock &require_update() = 0;
	virtual UniformBuffer const &buffer() const = 0;
	virtual BlockResources *get_resource(std::string const &name) = 0;
	virtual Key<ProgramResource> get_key(std::string const &name) = 0;
	virtual BlockResources *get_resource(Key<ProgramResource> const &key) = 0;
	virtual ~IInterfaceBlock(){};

protected:
	IInterfaceBlock(){};
};