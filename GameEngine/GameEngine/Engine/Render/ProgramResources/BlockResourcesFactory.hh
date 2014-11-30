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
	BlockResourcesFactory();

public:
	std::unique_ptr<IBlockResources> operator()(GLenum mode, GLint id, std::string &&name);

private:
	std::vector<std::pair<GLenum, std::function<std::unique_ptr<IBlockResources> (GLint, std::string &&)>>> _blue_prints;
};