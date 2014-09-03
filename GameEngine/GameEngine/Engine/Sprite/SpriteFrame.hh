#pragma once

#include <glm/glm.hpp>
#include <Render/GeometryManager.hh>
#include <Render/Data.hh>
#include <array>

class SpriteManager;

class SpriteFrame
{
public:
	SpriteFrame()
		: _dimensions(0)
		, _uvs(0)
	{}

	~SpriteFrame()
	{
	}

	bool load(gl::GeometryManager *vm) 
	{
		_m = vm;
		AGE::Vector<glm::vec4>		vertices;	// vertices positions
		AGE::Vector<glm::vec2>		uvs;		// texture coordinates
		AGE::Vector<glm::vec4>		normals;
		AGE::Vector<glm::vec4>		colors;
		AGE::Vector<unsigned int>	indices;	// indices

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

		void *buffer[4] = { &vertices[0], &colors[0], &normals[0], &uvs[0] };
		size_t sizeBuffer[4] = { vertices.size() * 4 * sizeof(float),
								colors.size() * 4 * sizeof(float),
								normals.size() * 4 * sizeof(float),
								uvs.size() * 2 * sizeof(float) };
		_verticesbuffer = _m->addVertices(vertices.size(), 4, sizeBuffer, buffer);
		_indicesbuffer = _m->addIndices(indices.size(), &indices[0]);
		_m->attachIndicesToIndexPool(_indicesbuffer, _m->getIndexPool(0));
		_m->attachVerticesToVertexPool(_verticesbuffer, _m->getVertexPool(1));
		return true;
	}
	inline const glm::uvec4 &getDimensions() const { return _dimensions; }
	inline const glm::vec4 &getUvs() const { return _uvs; }
	inline void draw() const
	{
		_m->draw(GL_QUADS, _indicesbuffer, _verticesbuffer);
	}
//private:
	glm::uvec4 _dimensions;
	glm::vec4 _uvs;
	gl::GeometryManager *_m;
	gl::Key<gl::Vertices> _verticesbuffer;
	gl::Key<gl::Indices> _indicesbuffer;
	friend class SpriteManager;
};