#include <AssetFiles/AssetFileManager.hpp>
#include <FileUtils/FileSystemHelpers.hpp>
#include <AssetFiles/MeshRawFile.hpp>
#include <AssetFiles/TextureRawFile.hpp>
#include <AssetFiles/MaterialRawFile.hpp>
#include <FileUtils/AssetFiles/CookedFile.hpp>
#include <FileUtils/AssetFiles/AssetsTypes.hpp>
#include <LowLevelUtils/BitOperations.hpp>
#include <FileUtils/AssetFiles/Folder.hpp>
#include <imgui/imgui.h>
#include <vector>
#include <Convertor/AssetDataSet.hpp>

//to remove
#include <iostream>

namespace AGE
{
	namespace AE
	{
		static char* DirtyString = "Dirty";
		static std::vector<ImVec4> &Colors()
		{
			static auto colors = std::vector < ImVec4 > {
				ImVec4(0.8f, 0.8f, 0.8f, 1.0f), // raw
					ImVec4(0.7f, 1.0f, 0.85f, 1.0f), // cooked
					ImVec4(0.0f, 0.65f, 0.85f, 1.0f), // Mesh
					ImVec4(0.78f, 0.05f, 0.75f, 1.0f), // material
					ImVec4(0.9f, 0.6f, 0.0f, 1.0f) // texture
			};
			return colors;
		}

		static const char **EnumToString()
		{
			static const char *enumStrings[] = { "Raw", "Cooked", "Mesh", "Material", "Texure" };
			return enumStrings;
		}

		void AssetFileManager::CheckIfRawModified(FileUtils::Folder *folder, std::set<std::shared_ptr<FileUtils::RawFile>> &list)
		{
			folder->update(std::function<void(FileUtils::Folder*)>([](FileUtils::Folder*){}), std::function<void(FileUtils::RawFile*)>([&](FileUtils::RawFile* ptr) {
				if (ptr->dataSet && ptr->dataSet->isConverting)
					return;
				auto lastWrite = FileUtils::GetLastWriteTime(ptr->getPath());
				if (FileUtils::GetDiffTime(lastWrite, ptr->_lastWriteTime) > 0)
				{
					ptr->_dirty = true;
					list.insert(std::static_pointer_cast<FileUtils::RawFile>(ptr->getSharedPtrOnThis()));
				}
				else
					ptr->_dirty = false;
			}));
		}

		void AssetFileManager::PrintSelectableRawAssetsFile(FileUtils::RawFile *ptr, int printSections, std::set<std::shared_ptr<FileUtils::RawFile>> *list)
		{
			if (!ptr->_active)
				return;
			auto t = Bits::countOne(printSections);
			ImGui::Columns(t);
			if (list && printSections & Name)
			{
				if (ImGui::Checkbox(ptr->getFileName().c_str(), &ptr->_selected))
				{
					auto castedPtr = std::static_pointer_cast<FileUtils::RawFile>(ptr->getSharedPtrOnThis());
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
				if (ptr->_dirty)
				{
					ImGui::SameLine();
					ImGui::TextColored(ImVec4(1, 0, 0, 1), DirtyString);
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

		void AssetFileManager::PrintClickableRawAssetsFile(FileUtils::RawFile *ptr, int printSections, std::shared_ptr<FileUtils::RawFile> &selected)
		{
			if (!ptr->_active)
				return;
			auto t = Bits::countOne(printSections);
			ImGui::Columns(t + 1);

			ImGui::PushID((void*)ptr);
			if (ImGui::Button("Select"))
			{
				selected = std::static_pointer_cast<FileUtils::RawFile>(ptr->getSharedPtrOnThis());
			}
			ImGui::PopID();

			ImGui::NextColumn();

			if (printSections & Name)
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
				if (ptr->_dirty)
				{
					ImGui::SameLine();
					ImGui::TextColored(ImVec4(1, 0, 0, 1), DirtyString);
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

		void AssetFileManager::LinkRawToCooked(FileUtils::Folder *raw, FileUtils::Folder *cooked)
		{
			raw->update(std::function<void(FileUtils::RawFile *)>([&](FileUtils::RawFile *ptr){
				//if (!ptr->_cookedFile)
				{
					std::shared_ptr<FileUtils::AssetFile> result = nullptr;
					auto cookedPath = RawPathToCooked(FileUtils::CleanPath(ptr->getPath()));
					cooked->find(cookedPath, result);
					if (result)
					{
						ptr->_cookedFile = std::static_pointer_cast<FileUtils::CookedFile>(result->getSharedPtrOnThis());
						std::static_pointer_cast<FileUtils::CookedFile>(result)->_rawFile = std::static_pointer_cast<FileUtils::RawFile>(ptr->getSharedPtrOnThis());
						if (FileUtils::GetDiffTime(ptr->_lastWriteTime, result->_lastWriteTime) > 0)
						{
							std::cout << FileUtils::GetDateStr(ptr->getPath()) << " | " << FileUtils::GetDateStr(result->getPath()) << std::endl;
							//TODO push in dirty list
							ptr->_dirty = true;
						}
						else
						{
							ptr->_dirty = false;
						}
					}
					else
					{
						ptr->_dirty = true;
					}
				}
			}));
		}

		std::string AssetFileManager::RawPathToCooked(const std::string &path)
		{
			auto index = path.find("/Raw/");
			if (index == std::string::npos)
				return path;
			auto res = path;
			res.replace(index, 5, "/Serialized/");
			auto extension = FileUtils::GetExtension(res);
			if (extension == "obj" || extension == "fbx" || extension == "collada")
			{
				res.replace(res.find("." + extension), extension.size() + 1, ".sage");
			}
			else if (extension == "bmp" || extension == "jpg" || extension == "jpeg" || extension == "tga" || extension == "png")
			{
				res.replace(res.find("." + extension), extension.size() + 1, ".dds");
			}
			else if (extension == "mtl")
			{
				res.replace(res.find("." + extension), extension.size() + 1, ".mage");
			}
			return res;
		}
	}
}
