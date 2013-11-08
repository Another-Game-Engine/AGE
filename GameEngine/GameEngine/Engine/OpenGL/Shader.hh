//
// Shader.hh for  in /home/massora/GIT/amd_project/render/ShaderTool
// 
// Made by dorian pinaud
// Login   <pinaud_d@epitech.net>
// 
// Started on  Mon Aug  5 00:33:58 2013 dorian pinaud
// Last update Sun Aug 18 17:17:51 2013 dorian pinaud
//

#ifndef SHADER_HH_
# define SHADER_HH_

# include "Utils/OpenGL.hh"
# include "OpenGL/UniformBuffer.hh"

# include "glm/glm.hpp"

# include <iostream>
# include <map>
# include <vector>
# include <fstream>
# include <string>
# include <set>

namespace OpenGLTools
{

// This class is templated on the number of uniformBuffers contained
class Shader
{
private:
	GLuint						_progId;
	GLuint						_vertexId;
	GLuint						_fragId;
	GLuint						_geometryId;
	GLenum                      *_targets;
	GLuint                      _textureNumber;
	std::set<GLenum>            _targetsList;
	std::set<GLenum>      _layersList;

	void linkProgram(GLuint progId) const;
	void compileShader(GLuint shaderId, std::string const &file) const;
	GLuint addShader(std::string const &path, GLenum type);

public:
	Shader(void);
	~Shader(void);

	bool	init(std::string const &vertex, std::string const &fragment, std::string const &geometry = "");
	bool	bindUniformBlock(std::string const &blockName, UniformBuffer const &buff);
	Shader	&bindActiveTexture(std::string const &uniformName, GLuint activeTexture);

	void	use(void);
	GLuint	getId() const;
	GLenum  *getTargets() const;
	unsigned int getTargetsNumber() const;
	bool build();
	Shader &addTarget(GLenum target);
	Shader &removeTarget(GLenum target);
	void clearTargets();
	Shader &addLayer(GLenum layer);
	Shader &removeLayer(GLenum layer);
	void clearLayers();
	inline const std::set<GLenum> &getLayers() const {return _layersList;}
	inline Shader &setTextureNumber(unsigned int t){_textureNumber = t; return *this;}
};

}

#endif /*!SHADER_HH_*/
