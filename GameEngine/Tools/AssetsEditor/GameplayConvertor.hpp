#pragma once

#include <Utils/Dependency.hpp>
#include <set>

#include <fbx/Node.h>

#include <AGE_FBX/Node.hpp>

namespace AGE
{
	class GameplayConvertor : public Dependency
	{
	public:
		GameplayConvertor()
		{}

		virtual ~GameplayConvertor()
		{}

		bool importNode(gameplay::Node *gpNode)
		{
			auto debug = _nodes.size();

			if (_nodes.find(gpNode->getId()) != std::end(_nodes))
				return false;
			if (gpNode->isJoint())
			{
				return _importSkeletonNode(gpNode);
			}
			else if (gpNode->_model != nullptr)
			{
				return _importModelNode(gpNode);
				std::cout << "implement me here please cesar ! Or anybody else ! Ho ... wait, there's not anybody else :)" << std::endl;
			}
			return false;
		}

	private:
		bool _importSkeletonNode(gameplay::Node *gpNode)
		{
			auto node = new AGE::Node();

			node->_id = _nodes.size();
			node->_name = gpNode->getId();
			node->_boneMatrixReference = gpNode->_transform;
			node->_worldMatrixReference = gpNode->_worldTransform;

			_nodes.insert(std::make_pair(node->_name, node));
			_debugVector.push_back(node);
			if (gpNode->hasChildren())
			{
				auto child = gpNode->getFirstChild();
				do
				{
					assert(child != nullptr && child->isJoint());
					if (_importSkeletonNode(child) == false)
						return false;
					auto siblingIt = _nodes.find(child->getId());
					assert(siblingIt != std::end(_nodes));
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
			auto m = gpNode->_model->getMesh();

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

			for (std::size_t p = 0; p < m->parts.size(); ++p)
			{
				for (std::size_t i = 0; i < m->parts[p]->_indices.size(); ++i)
				{
					indices.push_back(m->parts[p]->_indices[i]);
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
			
			Data indicesData(indices.size() * sizeof(unsigned int), &indices[0]);
			vertices = new Vertice<8>(indices.size(), data, &indicesData);
			this->_dpyManager.lock()->getInstance<VertexManager<8>>()->addVertice(*vertices);

			mesh = new AGE::Mesh<8>(*vertices);
			return true;
		}
public:
	AGE::Mesh<8> *mesh;
	Vertice<8> *vertices;
	AGE::Node *skeletonBase;
private:
		std::map<std::string, AGE::Node*> _nodes;
		std::vector<AGE::Node*> _debugVector;
	};
}