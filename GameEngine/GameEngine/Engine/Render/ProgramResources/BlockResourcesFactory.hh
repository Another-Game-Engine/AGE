#pragma once

# include <Utils/OpenGL.hh>
# include <vector>
# include <utility>
# include <memory>
# include <functional>
# include <Render/ProgramResources/IBlockResources.hh>

class BlockResourcesFactory
{
public:
	BlockResourcesFactory(UniformBlock const &parent);

public:
	std::unique_ptr<IBlockResources> operator()(GLenum mode, GLint id);

private:
	UniformBlock const &_parent;
	std::vector<std::pair<GLenum, std::function<std::unique_ptr<IBlockResources> (GLint)>>> _blue_prints;
};