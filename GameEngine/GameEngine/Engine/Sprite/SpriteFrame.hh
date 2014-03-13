#pragma once

#include <glm/glm.hpp>
#include <OpenGL/VertexManager.hh>
#include <OpenGL/Vertice.hh>
#include <array>

class SpriteManager;

class SpriteFrame
{
public:
	SpriteFrame()
		: _dimensions(0)
		, _uvs(0)
		, _buffer(nullptr)
	{}

	~SpriteFrame()
	{
		if (_buffer)
			delete _buffer;
	}
	bool load(std::unique_ptr<VertexManager<2>> &vm) 
	{
		std::vector<glm::vec4>		vertices;	// vertices positions
		std::vector<glm::vec2>		uvs;		// texture coordinates
		std::vector<unsigned int>	indices;	// indices

		vertices.resize(4);
		uvs.resize(4);
		indices.resize(4);
		indices = { 0, 1, 2, 3 };

		vertices[0] = glm::vec4(_dimensions[0], _dimensions[1], 0, 1);
		vertices[1] = glm::vec4(_dimensions[2], _dimensions[1], 0, 1);
		vertices[2] = glm::vec4(_dimensions[2], _dimensions[3], 0, 1);
		vertices[3] = glm::vec4(_dimensions[0], _dimensions[3], 0, 1);

		uvs[0] = glm::vec2(_uvs[0], _uvs[1]);
		uvs[1] = glm::vec2(_uvs[2], _uvs[1]);
		uvs[2] = glm::vec2(_uvs[2], _uvs[3]);
		uvs[3] = glm::vec2(_uvs[0], _uvs[3]);

		std::array<Data, 2> data =
		{
			Data(vertices.size() * 4 * sizeof(float), &vertices[0].x),
			Data(uvs.size() * 2 * sizeof(float), &uvs[0].x)
		};
		Data indicesData(indices.size() * sizeof(unsigned int), &indices[0]);
		_buffer = new Vertice<2>(vertices.size(), data, &indicesData);
		vm->addVertice(*(_buffer));
		return true;
	}
	inline const glm::uvec4 &getDimensions() const { return _dimensions; }
	inline const glm::vec4 &getUvs() const { return _uvs; }
	inline void draw() const { _buffer->draw(GL_QUADS); }
private:
	glm::uvec4 _dimensions;
	glm::vec4 _uvs;
	Vertice<2> *_buffer;
	friend class SpriteManager;
};