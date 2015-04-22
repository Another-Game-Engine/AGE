#include <AssetFiles/AssetFileManager.hpp>
#include <Utils/FileSystemHelpers.hpp>
#include <AssetFiles/MeshRawFile.hpp>
#include <AssetFiles/TextureRawFile.hpp>
#include <AssetFiles/MaterialRawFile.hpp>
#include <AssetFiles/CookedFile.hpp>
#include <AssetFiles/AssetsTypes.hpp>
#include <Utils/BitOperations.hpp>
#include <AssetFiles/Folder.hpp>
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

		bool AssetFileManager::IsValidFile(const std::tr2::sys::path &path)
		{
			auto extension = AGE::FileSystemHelpers::GetExtension(path);
			if (extension == "obj" || extension == "fbx" || extension == "dae")
				return true;
			if (extension == "bmp" || extension == "jpg" || extension == "jpeg" || extension == "tga" || extension == "png" || extension == "tif")
				return true;
			if (extension == "mtl")
				return true;
			return false;
		}

		std::shared_ptr<AssetFile> AssetFileManager::AgeCreateFile(const std::tr2::sys::path &path, Folder *parent)
		{
			auto extension = AGE::FileSystemHelpers::GetExtension(path);
			std::shared_ptr<AssetFile> t = nullptr;
			if (extension == "obj" || extension == "fbx" || extension == "dae")
			{
				t = std::make_shared<MeshRawFile>(path, parent);
				t->_type = AssetType::Raw | AssetType::Mesh;
			}
			else if (extension == "bmp" || extension == "jpg" || extension == "jpeg" || extension == "tga" || extension == "png" || extension == "tif")
			{
				t = std::make_shared<TextureRawFile>(path, parent);
				t->_type = AssetType::Raw | AssetType::Texture;
			}
			else if (extension == "mtl")
			{
				t =  std::make_shared<MaterialRawFile>(path, parent);
				t->_type = AssetType::Raw | AssetType::Material;
			}

			if (t)
			{
				t->_lastWriteTime = AGE::FileSystemHelpers::GetLastWriteTime(t->getPath());
				t->_lastWriteTimeStr = AGE::FileSystemHelpers::GetDateStr(t->getPath());
			}
			return t;
		}

		std::shared_ptr<AssetFile> AssetFileManager::AgeCreateFile(const std::string &path, Folder *parent)
		{
			return AgeCreateFile(std::tr2::sys::path(path), parent);
		}

		void AssetFileManager::CheckIfRawModified(Folder *folder, std::set<std::shared_ptr<RawFile>> &list)
		{
			folder->update(std::function<void(Folder*)>([](Folder*){}), std::function<void(RawFile*)>([&](RawFile* ptr) {
				if (ptr->dataSet && ptr->dataSet->isConverting)
					return;
				auto lastWrite = FileSystemHelpers::GetLastWriteTime(ptr->getPath());
				if (FileSystemHelpers::GetDiffTime(lastWrite, ptr->_lastWriteTime) > 0)
				{
					ptr->_dirty = true;
					list.insert(std::static_pointer_cast<RawFile>(ptr->getSharedPtrOnThis()));
				}
				else
					ptr->_dirty = false;
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
				if (ImGui::Checkbox(ptr->getFileName().c_str(), &ptr->_selected))
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

		void AssetFileManager::PrintClickableRawAssetsFile(RawFile *ptr, int printSections, std::shared_ptr<RawFile> &selected)
		{
			if (!ptr->_active)
				return;
			auto t = Bits::countOne(printSections);
			ImGui::Columns(t + 1);

			ImGui::PushID((void*)ptr);
			if (ImGui::Button("Select"))
			{
				selected = std::static_pointer_cast<RawFile>(ptr->getSharedPtrOnThis());
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

		void AssetFileManager::LinkRawToCooked(Folder *raw, Folder *cooked)
		{
			raw->update(std::function<void(RawFile *)>([&](RawFile *ptr){
				//if (!ptr->_cookedFile)
				{
					std::shared_ptr<AGE::AE::AssetFile> result = nullptr;
					auto cookedPath = RawPathToCooked(FileSystemHelpers::CleanPath(ptr->getPath()));
					cooked->find(cookedPath, result);
					if (result)
					{
						ptr->_cookedFile = std::static_pointer_cast<CookedFile>(result->getSharedPtrOnThis());
						std::static_pointer_cast<CookedFile>(result)->_rawFile = std::static_pointer_cast<RawFile>(ptr->getSharedPtrOnThis());
						if (FileSystemHelpers::GetDiffTime(ptr->_lastWriteTime, result->_lastWriteTime) > 0)
						{
							std::cout << FileSystemHelpers::GetDateStr(ptr->getPath()) << " | " << FileSystemHelpers::GetDateStr(result->getPath()) << std::endl;
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
			auto extension = FileSystemHelpers::GetExtension(res);
			if (extension == "obj" || extension == "fbx" || extension == "collada")
			{
				res.replace(res.find("." + extension), extension.size() + 1, ".sage");
			}
			else if (extension == "bmp" || extension == "jpg" || extension == "jpeg" || extension == "tga" || extension == "png")
			{
				res.replace(res.find("." + extension), extension.size() + 1, ".tage");
			}
			else if (extension == "mtl")
			{
				res.replace(res.find("." + extension), extension.size() + 1, ".mage");
			}
			return res;
		}
	}
}
