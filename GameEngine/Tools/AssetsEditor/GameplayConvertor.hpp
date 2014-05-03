#pragma once

#include <Utils/Dependency.hpp>
#include <set>

#include <fbx/Node.h>

#include <AGE_FBX/Node.hpp>


static inline glm::mat4 GP_MAT_TO_GLM(const gameplay::Matrix &m)
{
	return glm::mat4(m.m[0], m.m[1], m.m[2], m.m[3], m.m[4], m.m[5], m.m[6], m.m[7], m.m[8], m.m[9], m.m[10], m.m[11], m.m[12], m.m[13], m.m[14], m.m[15]);
	//return glm::mat4(m.m[0], m.m[4], m.m[8], m.m[12], m.m[1], m.m[5], m.m[9], m.m[13], m.m[2], m.m[6], m.m[10], m.m[14], m.m[3], m.m[7], m.m[11], m.m[15]);
}

namespace AGE
{
	struct AnimationChannel
	{

	};

	class GameplayConvertor : public Dependency
	{
	public:
		GameplayConvertor()
		{}

		virtual ~GameplayConvertor()
		{}

		bool importGPFile(gameplay::GPBFile &file)
		{
			if (!_importModelNode(file._geometry.front()))
				return false;
			return true;
		}

		void update(float time)
		{
			static float t = 0;
			bonesMatrix.clear();
			bonesMatrix.resize(_bindPoses.size());
			for (auto i = 0; i < _bindPoses.size(); ++i)
			{
				bonesMatrix[i] = _worldPoses[i] * _bindPoses[i] * _bindShape;
			}
			t += time;
		}

	private:
		bool _importModelNode(gameplay::Mesh *m)
		{
			std::vector<glm::vec4> positions;
			std::vector<glm::vec4> normals;
			std::vector<glm::vec4> tangents;
			std::vector<glm::vec4> binormals;
			std::vector<glm::vec2> texCoords;
			std::vector<glm::vec4> diffuses;
			std::vector<glm::vec4> blendWeights;
			std::vector<glm::vec4> blendIndices;
			std::vector<unsigned int> indices;

			for (std::size_t i = 0; i < m->vertices.size(); ++i)
			{
				positions.push_back(glm::vec4(m->vertices[i].position.x, m->vertices[i].position.y, m->vertices[i].position.z, 1));
				normals.push_back(glm::vec4(m->vertices[i].normal.x, m->vertices[i].normal.y, m->vertices[i].normal.z, 1));
				tangents.push_back(glm::vec4(m->vertices[i].tangent.x, m->vertices[i].tangent.y, m->vertices[i].tangent.z, 1));
				binormals.push_back(glm::vec4(m->vertices[i].binormal.x, m->vertices[i].binormal.y, m->vertices[i].binormal.z, 1));
				// @dorian Ici il peut il y avoir plusieurs coordonnees de textures, faut que le vertex manager puisse gerer ca
				texCoords.push_back(glm::vec2(m->vertices[i].texCoord[0].x, m->vertices[i].texCoord[0].y));
				diffuses.push_back(glm::vec4(m->vertices[i].binormal.x, m->vertices[i].binormal.y, m->vertices[i].binormal.z, 1));
				blendWeights.push_back(glm::vec4(m->vertices[i].blendWeights.x, m->vertices[i].blendWeights.y, m->vertices[i].blendWeights.z, m->vertices[i].blendWeights.w));
				blendIndices.push_back(glm::vec4(m->vertices[i].blendIndices.x, m->vertices[i].blendIndices.y, m->vertices[i].blendIndices.z, m->vertices[i].blendIndices.w));
			}
			std::size_t max = 0;
			for (std::size_t p = 0; p < m->parts.size(); ++p)
			{
				for (std::size_t i = 0; i < m->parts[p]->getIndices().size(); ++i)
				{
					indices.push_back(m->parts[p]->getIndices()[i]);
					if (m->parts[p]->getIndices()[i] > max)
						max = m->parts[p]->getIndices()[i];
				}
			}

			std::array<Data, 8> data =
			{
				Data(positions.size() * 4 * sizeof(float), &positions[0].x)
				, Data(normals.size() * 4 * sizeof(float), &normals[0].x)
				, Data(tangents.size() * 4 * sizeof(float), &tangents[0].x)
				, Data(binormals.size() * 4 * sizeof(float), &binormals[0].x)
				, Data(texCoords.size() * 2 * sizeof(float), &texCoords[0].x)
				, Data(diffuses.size() * 4 * sizeof(float), &diffuses[0].x)
				, Data(blendWeights.size() * 4 * sizeof(float), &blendWeights[0].x)
				, Data(blendIndices.size() * 4 * sizeof(float), &blendIndices[0].x)
			};
			
			Data *indicesData = new Data(indices.size() * sizeof(unsigned int), &indices[0]);
			vertices = new Vertice<8>(positions.size(), data, indicesData);
			this->_dpyManager.lock()->getInstance<VertexManager<8>>()->addVertice(*vertices);

			mesh = new AGE::Mesh<8>(*vertices);


			for (auto b : m->model->getSkin()->getBindPoses())
			{
				_bindPoses.push_back(GP_MAT_TO_GLM(b));
			}

			for (auto w : m->model->getSkin()->getJoints())
			{
				_worldPoses.push_back(GP_MAT_TO_GLM(w->getWorldMatrix()));
//				_bonePoses.push_back(GP_MAT_TO_GLM(w->get))
			}

			auto t = m->model->getSkin()->_bindShape;
			_bindShape = glm::mat4(t[0], t[1], t[2], t[3], t[4], t[5], t[6], t[7], t[8], t[9], t[10], t[11], t[12], t[13], t[14], t[15]);

			return true;
		}
public:
	AGE::Mesh<8> *mesh;
	Vertice<8> *vertices;
	std::vector<glm::mat4> bonesMatrix;
private:
		gameplay::GPBFile *_file;
		std::vector<glm::mat4> _bindPoses;
		std::vector<glm::mat4> _worldPoses;
		glm::mat4 _bindShape;
	};
}