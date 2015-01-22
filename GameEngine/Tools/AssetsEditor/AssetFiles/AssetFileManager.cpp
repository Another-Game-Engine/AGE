#include <AssetFiles/AssetFileManager.hpp>
#include <Utils/FileSystem.hpp>
#include <AssetFiles/RawFile.hpp>
#include <AssetFiles/AssetsTypes.hpp>
#include <Utils/BitOperations.hpp>
#include <AssetFiles/Folder.hpp>
#include <imgui/imgui.h>
#include <vector>

namespace AGE
{
	namespace AE
	{
		static std::vector<ImVec4> &Colors()
		{
			static auto colors = std::vector < ImVec4 > {
				ImVec4(0.8, 0.8, 0.8, 1), // raw
					ImVec4(0.7, 1, 0.85, 1), // cooked
					ImVec4(0, 0.65, 0.85, 1), // Mesh
					ImVec4(0.78, 0.05, 0.75, 1), // material
					ImVec4(0.9, 0.6, 0, 1) // texture
			};
			return colors;
		}

		static const char **EnumToString()
		{
			static const char *enumStrings[] = { "Raw", "Cooked", "Mesh", "Material", "Texure" };
			return enumStrings;
		}
		std::shared_ptr<AssetFile> AssetFileManager::CreateFile(const std::tr2::sys::path &path, Folder *parent)
		{
			auto extension = AGE::FileSystem::GetExtension(path);
			std::shared_ptr<AssetFile> t = nullptr;
			if (extension == "obj" || extension == "fbx" || extension == "collada")
			{
				t = std::make_shared<RawFile>(path, parent);
				t->_type = AssetType::Raw | AssetType::Mesh;
			}
			else if (extension == "bmp" || extension == "jpg" || extension == "jpeg" || extension == "tga" || extension == "png")
			{
				t = std::make_shared<RawFile>(path, parent);
				t->_type = AssetType::Raw | AssetType::Texture;

			}
			else if (extension == "mtl")
			{
				t =  std::make_shared<RawFile>(path, parent);
				t->_type = AssetType::Raw | AssetType::Material;

			}
			if (t)
			{
				t->_lastWriteTime = AGE::FileSystem::GetLastWriteTime(t->getPath());
				t->_lastWriteTimeStr = AGE::FileSystem::GetDateStr(t->getPath());
			}
			return t;
		}

		std::shared_ptr<AssetFile> AssetFileManager::CreateFile(const std::string &path, Folder *parent)
		{
			return CreateFile(std::tr2::sys::path(path), parent);
		}

		void AssetFileManager::CheckIfRawModified(Folder *folder, std::set<std::shared_ptr<RawFile>> &list)
		{
			folder->update(std::function<void(Folder*)>([](Folder*){}), std::function<void(RawFile*)>([&](RawFile* ptr) {
				auto lastWrite = FileSystem::GetLastWriteTime(ptr->getPath());
				if (FileSystem::GetDiffTime(lastWrite, ptr->_lastWriteTime) > 0)
				{
					ptr->_dirty = true;
					ptr->_lastWriteTime = lastWrite;
					ptr->_lastWriteTimeStr = FileSystem::GetDateStr(ptr->getPath());
					list.insert(std::static_pointer_cast<RawFile>(ptr->getSharedPtrOnThis()));
				}
			}));
		}

		void AssetFileManager::PrintSelectableRawAssetsFile(RawFile *ptr, int printSections, std::set<std::shared_ptr<RawFile>> *list)
		{
			if (!ptr->_active)
				return;
			auto t = Bits::countOne(printSections);
			ImGui::Columns(t);
			if (list && printSections & Name)
			{
				if (ImGui::Checkbox(ptr->getPath().c_str(), &ptr->_selected))
				{
					auto castedPtr = std::static_pointer_cast<RawFile>(ptr->getSharedPtrOnThis());
					if (ptr->_selected)
					{
						list->insert(castedPtr);
					}
					else
					{
						auto f = list->find(castedPtr);
						if (f != list->end())
							list->erase(f);
					}
				}
				ImGui::NextColumn();
			}
			else if (printSections & Name)
			{
				ImGui::Text(ptr->getFileName().c_str());
				ImGui::NextColumn();
			}
			if (printSections & Type)
			{
				int i; int type = ptr->_type;
				while (type)
				{
					i = Bits::getFirstBitPosition(type);
					type ^= 1 << i;
					ImGui::TextColored(Colors()[i], EnumToString()[i]);
					if (type)
						ImGui::SameLine();
				}
				ImGui::NextColumn();
			}
			if (printSections & Date)
			{
				ImGui::Text(ptr->_lastWriteTimeStr.c_str());
				ImGui::NextColumn();
			}
			if (printSections & Path)
			{
				ImGui::Text(ptr->getFileName().c_str());
				ImGui::NextColumn();
			}
			ImGui::Columns(1);
		}
	}
}
