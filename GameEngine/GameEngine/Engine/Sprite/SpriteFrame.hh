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
#if !TEST_NEW_VERTEXMANAGER
		, _buffer(nullptr)
#endif
	{}

	~SpriteFrame()
	{
#if !TEST_NEW_VERTEXMANAGER
		if (_buffer)
			delete _buffer;
#endif
	}
#if !TEST_NEW_VERTEXMANAGER
	bool load(std::shared_ptr<VertexManager<4>> vm) 
#else
	bool load()
#endif
	{
		std::vector<glm::vec4>		vertices;	// vertices positions
		std::vector<glm::vec2>		uvs;		// texture coordinates
		std::vector<glm::vec4>		normals;
		std::vector<glm::vec4>		colors;
		std::vector<unsigned int>	indices;	// indices

		vertices.resize(4); //-V112
		uvs.resize(4); //-V112
		normals.resize(4); //-V112
		colors.resize(4); //-V112
		indices.resize(4); //-V112
		indices = { 0, 1, 2, 3 };

		float c = (float)_dimensions[3] / (float)_dimensions[2];

		vertices[0] = glm::vec4(-0.5f, -0.5f * c, 0, 1);
		vertices[1] = glm::vec4(-0.5f, 0.5f * c, 0, 1);
		vertices[2] = glm::vec4(0.5f, 0.5f * c, 0, 1);
		vertices[3] = glm::vec4(0.5f, -0.5f * c, 0, 1);

		uvs[0] = glm::vec2(_uvs[2], _uvs[3]);
		uvs[1] = glm::vec2(_uvs[2], _uvs[1]);
		uvs[2] = glm::vec2(_uvs[0], _uvs[1]);
		uvs[3] = glm::vec2(_uvs[0], _uvs[3]);

		normals[0] = glm::vec4(0, 0, -1, 0);
		normals[1] = glm::vec4(0, 0, -1, 0);
		normals[2] = glm::vec4(0, 0, -1, 0);
		normals[3] = glm::vec4(0, 0, -1, 0);

		colors[0] = glm::vec4(1);
		colors[1] = glm::vec4(1);
		colors[2] = glm::vec4(1);
		colors[3] = glm::vec4(1);
#if	!TEST_NEW_VERTEXMANAGER
		std::array<Data, 4> data =
		{
			Data(vertices.size() * 4 * sizeof(float), &vertices[0].x),
			Data(colors.size() * 4 * sizeof(float), &colors[0].x),
			Data(normals.size() * 4 * sizeof(float), &normals[0].x),
			Data(uvs.size() * 2 * sizeof(float), &uvs[0].x)
		};
		Data indicesData(indices.size() * sizeof(unsigned int), &indices[0]);
		_buffer = new Vertice<4>(vertices.size(), data, &indicesData);
		vm->addVertice(*(_buffer));
#endif
		return true;
	}
	inline const glm::uvec4 &getDimensions() const { return _dimensions; }
	inline const glm::vec4 &getUvs() const { return _uvs; }
	inline void draw() const
	{
#if	!TEST_NEW_VERTEXMANAGER
		_buffer->draw(GL_QUADS);
#endif
	}
//private:
	glm::uvec4 _dimensions;
	glm::vec4 _uvs;
#if !TEST_NEW_VERTEXMANAGER
	Vertice<4> *_buffer;
#endif
	friend class SpriteManager;
};