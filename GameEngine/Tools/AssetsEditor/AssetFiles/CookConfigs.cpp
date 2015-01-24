#include <CookConfigs.hpp>
#include <Utils/Age_Imgui.hpp>

namespace AGE
{
	namespace AE
	{
		const char *PhysicConfig::_typeString[] = { "staticConcave", "dynamicConcave", "sphere", "cube" };
		const char *CookConfig::_cookConfigTypeString[] = { "Animation", "Material",  "Skin", "Physic", "Skeleton", "Texture", "UNKNOWN" };

		CookConfig::CookConfig(CookConfigType t)
			: type(t)
		{
			memset(name, 0, sizeof(char) * MAX_ASSET_NAME_LENGTH);
		}

		void CookConfig::update()
		{
			modified = 0;
			ImGui::Text("Type : %s", cookConfigTypeToString());
			modified += ImGui::InputText("Name", name, MAX_ASSET_NAME_LENGTH);
			if (ImGui::TreeNode("Last cooking logs :"))
			{
				ImGui::Text(logs.c_str());
				ImGui::TreePop();
			}
			drawImGuiTypeSpecific();
			if (modified > 0)
				updateLastTimeEdited();
		}

		void CookConfig::updateLastTimeEdited()
		{
			time_t t;
			time(&t);
			lastTimeEdited = *localtime(&t);
		}


		AnimationConfig::AnimationConfig()
			: CookConfig(CookConfigType::Animation)
		{
		}

		void AnimationConfig::drawImGuiTypeSpecific()
		{}

		MaterialConfig::MaterialConfig()
			: CookConfig(CookConfigType::Material)
		{
		}
		
		void MaterialConfig::drawImGuiTypeSpecific()
		{
		}

		SkinConfig::SkinConfig()
			: CookConfig(CookConfigType::Skin)
		{
		}

		void SkinConfig::drawImGuiTypeSpecific()
		{
			if (ImGui::Checkbox("Normalize size", &normalize))
			{
				modified++;
				ImGui::SliderFloat("Max size length", &maxSideLength, 0.00001f, 1000.0f, "%.3f", 1.0f);
			}
			modified += ImGui::Checkbox("Psitions", &positions);
			modified += ImGui::Checkbox("Normals", &normals);
			modified += ImGui::Checkbox("Bones infos", &bonesInfos);
			modified += ImGui::Checkbox("Texture coordinates", &uvs);
			modified += ImGui::Checkbox("Tangents", &tangents);
			modified += ImGui::Checkbox("BiTangents", &biTangents);
		}

		PhysicConfig::PhysicConfig()
			: CookConfig(CookConfigType::Physic)
		{
			modified += ImGui::Combo("Rigid body type", (int*)&rigidBodyType, PhysicConfig::cookConfigTypeToString());
		}

		void PhysicConfig::drawImGuiTypeSpecific()
		{}

		SkeletonConfig::SkeletonConfig()
			: CookConfig(CookConfigType::Skeleton)
		{
		}

		void SkeletonConfig::drawImGuiTypeSpecific()
		{}

		TextureConfig::TextureConfig()
			: CookConfig(CookConfigType::Texture)
		{
		}

		void TextureConfig::drawImGuiTypeSpecific()
		{}
	}
}