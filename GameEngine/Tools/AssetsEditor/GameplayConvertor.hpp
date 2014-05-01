#pragma once

#include <Utils/Dependency.hpp>
#include <set>

#include <fbx/Node.h>

#include <AGE_FBX/Node.hpp>


static inline glm::mat4 GP_MAT_TO_GLM(const gameplay::Matrix &m)
{
	return glm::mat4(m.m[0], m.m[1], m.m[2], m.m[3], m.m[4], m.m[5], m.m[6], m.m[7], m.m[8], m.m[9], m.m[10], m.m[11], m.m[12], m.m[13], m.m[14], m.m[15]);
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
			gameplay::Object* object = file.getObjects().front();

			// Import bones and mesh
			for (auto n = file.getNodes().begin(); n != std::end(file.getNodes()); ++n)
			{
				_importNode(*n);
			}

			// Import Animation channel
			return true;
		}

		void update()
		{
			bonesMatrix.clear();
			for (auto e : _bones)
			{
				bonesMatrix.push_back(glm::mat4(1));
			}
		}

	private:
		bool _importNode(gameplay::Node *gpNode)
		{
			if (_bonesByName.find(gpNode->getId()) != std::end(_bonesByName))
				return false;
			if (gpNode->isJoint())
			{
				return _importSkeletonNode(gpNode);
			}
			else if (gpNode->getModel() != nullptr)
			{
				return _importModelNode(gpNode);
			}
			return false;
		}

		bool _importSkeletonNode(gameplay::Node *gpNode)
		{
			auto node = new AGE::Node();

			node->_id = _bonesByName.size();
			node->_name = gpNode->getId();
			node->_boneMatrixReference = GP_MAT_TO_GLM(gpNode->getTransformMatrix());
			node->_worldMatrixReference = GP_MAT_TO_GLM(gpNode->getWorldMatrix());

			_bonesByName.insert(std::make_pair(node->_name, node));
			_bones.push_back(node);
			if (gpNode->hasChildren())
			{
				auto child = gpNode->getFirstChild();
				do
				{
					assert(child != nullptr && child->isJoint());
					if (_importSkeletonNode(child) == false)
						return false;
					auto siblingIt = _bonesByName.find(child->getId());
					assert(siblingIt != std::end(_bonesByName));
					auto sibling = siblingIt->second;
					node->addChild(sibling->_id);
					sibling->setParent(node->_id);

					child = child->getNextSibling();
				} while (child != nullptr);
			}
			else
			{
				skeletonBase = node;
			}
			return true;
		}

		bool _importModelNode(gameplay::Node *gpNode)
		{
			auto m = gpNode->getModel()->getMesh();

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

			std::array<Data, 7> data =
			{
				Data(positions.size() * 4 * sizeof(float), &positions[0].x)
				, Data(normals.size() * 4 * sizeof(float), &normals[0].x)
				, Data(tangents.size() * 4 * sizeof(float), &tangents[0].x)
				, Data(binormals.size() * 4 * sizeof(float), &binormals[0].x)
//				, Data(texCoords.size() * 2 * sizeof(float), &texCoords[0].x)
				, Data(diffuses.size() * 4 * sizeof(float), &diffuses[0].x)
				, Data(blendWeights.size() * 4 * sizeof(float), &blendWeights[0].x)
				, Data(blendIndices.size() * 4 * sizeof(float), &blendIndices[0].x)
			};
			
			Data *indicesData = new Data(indices.size() * sizeof(unsigned int), &indices[0]);
			vertices = new Vertice<7>(positions.size(), data, indicesData);
			this->_dpyManager.lock()->getInstance<VertexManager<7>>()->addVertice(*vertices);

			mesh = new AGE::Mesh<7>(*vertices);
			return true;
		}
public:
	AGE::Mesh<7> *mesh;
	Vertice<7> *vertices;
	AGE::Node *skeletonBase;
	std::vector<glm::mat4> bonesMatrix;
private:
		std::map<std::string, AGE::Node*> _bonesByName;
		std::vector<AGE::Node*> _bones;
	};
}