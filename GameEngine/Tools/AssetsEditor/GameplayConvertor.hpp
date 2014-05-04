#pragma once

#include <Utils/Dependency.hpp>
#include <set>

#include <fbx/Node.h>

#include <AGE_FBX/Node.hpp>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

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
			for (auto i = 0; i < file.getAnimations()->getAnimation(0)->getAnimationChannelCount(); ++i)
			{
				_channels.push_back(file.getAnimations()->getAnimation(0)->getAnimationChannel(i));
			}
			return true;
		}

		void update(float time)
		{
			static float t = 0;
			bonesMatrix.clear();
			bonesMatrix.resize(_bindPoses.size());
			int random = (int)(t) / 4;
			if (random > 7)
				t = 0;
			std::cout << random << std::endl;
			for (auto i = 0; i < _bindPoses.size(); ++i)
			{
				if (random == 0)
					bonesMatrix[i] = _worldPoses[i] * _bindPoses[i] * _bindShape;
				else if (random == 1)
					bonesMatrix[i] = _worldPoses[i] * _bindPoses[i];
				else if (random == 2)
					bonesMatrix[i] = _worldPoses[i];
				else if (random == 3)
					bonesMatrix[i] = _worldPoses[i] * _bindShape;
				else if (random == 4)
					bonesMatrix[i] = _bindShape;
				else if (random == 5)
					bonesMatrix[i] = _bindPoses[i] * _bindShape;
				else if (random == 6)
					bonesMatrix[i] = _bindPoses[i];
				else
					bonesMatrix[i] = glm::mat4(1);
			}

			for (auto i = 0; i < _channels.size(); ++i)
			{
				std::string targetName = _channels[i]->getTargetId();
				auto targetIndex = 0;
				while (targetIndex < _boneNames.size())
				{
					if (_boneNames[targetIndex] == targetName)
						break;
					++targetIndex;
				}

				auto &m = bonesMatrix[targetIndex];
				auto &c = _channels[targetIndex];
				switch (c->getTargetAttribute())
				{
				case gameplay::Transform::ANIMATE_TRANSLATE:
					m = glm::translate(m, glm::vec3(c->getKeyValues()[0], c->getKeyValues()[1], c->getKeyValues()[2]));
					break;
				//case gameplay::Transform::ANIMATE_TRANSLATE_X:
				//	m = glm::translate(m, glm::vec3(c->getKeyValues()[0], 0, 0));
				//	break;
				//case gameplay::Transform::ANIMATE_TRANSLATE_Y:
				//	m = glm::translate(m, glm::vec3(0, c->getKeyValues()[0], 0));
				//	break;
				//case gameplay::Transform::ANIMATE_TRANSLATE_Z:
				//	m = glm::translate(m, glm::vec3(0, 0, c->getKeyValues()[0]));
				//	break;
				//case gameplay::Transform::ANIMATE_ROTATE:
				//	m *= glm::mat4_cast(glm::quat(c->getKeyValues()[3], c->getKeyValues()[0], c->getKeyValues()[1], c->getKeyValues()[2]));
				//	break;
				//case gameplay::Transform::ANIMATE_ROTATE_TRANSLATE:
				//	m *= glm::mat4_cast(glm::quat(c->getKeyValues()[3], c->getKeyValues()[0], c->getKeyValues()[1], c->getKeyValues()[2]));
				//	m = glm::translate(m, glm::vec3(c->getKeyValues()[4], c->getKeyValues()[5], c->getKeyValues()[6]));
				//	break;
				//case gameplay::Transform::ANIMATE_SCALE:
				//	m = glm::scale(m, glm::vec3(c->getKeyValues()[0], c->getKeyValues()[1], c->getKeyValues()[2]));
				//	break;
				//case gameplay::Transform::ANIMATE_SCALE_ROTATE:
				//	m = glm::scale(m, glm::vec3(c->getKeyValues()[0], c->getKeyValues()[1], c->getKeyValues()[2]));
				//	m *= glm::mat4_cast(glm::quat(c->getKeyValues()[6], c->getKeyValues()[3], c->getKeyValues()[4], c->getKeyValues()[5]));
				//	break;
				//case gameplay::Transform::ANIMATE_SCALE_ROTATE_TRANSLATE:
				//	m = glm::scale(m, glm::vec3(c->getKeyValues()[0], c->getKeyValues()[1], c->getKeyValues()[2]));
				//	m *= glm::mat4_cast(glm::quat(c->getKeyValues()[6], c->getKeyValues()[3], c->getKeyValues()[4], c->getKeyValues()[5]));
				//	m = glm::translate(m, glm::vec3(c->getKeyValues()[7], c->getKeyValues()[8], c->getKeyValues()[9]));
				//	break;
				//case gameplay::Transform::ANIMATE_SCALE_TRANSLATE:
				//	m = glm::scale(m, glm::vec3(c->getKeyValues()[0], c->getKeyValues()[1], c->getKeyValues()[2]));
				//	m = glm::translate(m, glm::vec3(c->getKeyValues()[3], c->getKeyValues()[4], c->getKeyValues()[5]));
				//	break;
				//case gameplay::Transform::ANIMATE_SCALE_X:
				//	m = glm::scale(m, glm::vec3(c->getKeyValues()[0], 1, 1));
				//	break;
				//case gameplay::Transform::ANIMATE_SCALE_Y:
				//	m = glm::scale(m, glm::vec3(1, c->getKeyValues()[0], 1));
				//	break;
				//case gameplay::Transform::ANIMATE_SCALE_Z:
				//	m = glm::scale(m, glm::vec3(1, 1, c->getKeyValues()[0]));
				//	break;
				default:
					break;
				}

			}

			t += time;

			//for (auto i = 0; i < _boneNames.size(); ++i)
			//{
			//}

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
				_boneNames.push_back(w->getId());
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
		std::vector<std::string> _boneNames;
		std::vector<gameplay::AnimationChannel*> _channels;
	};
}