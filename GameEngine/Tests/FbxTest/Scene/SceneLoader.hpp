#pragma once

#include <fbxsdk.h>
#include <string>
#include <iostream>

#include "../FbxMesh.hpp"

namespace AGE
{
	class SceneLoader
	{
	public:
		SceneLoader(const std::string &filePath)
			: _filePath(filePath)
		{

		}

		~SceneLoader()
		{}

		bool load()
		{
			//The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
			mSdkManager = FbxManager::Create();
			if (!mSdkManager)
			{
				_status = "Error: Unable to create FBX Manager!\n";
				return false;
			}
			std::cout << "Autodesk FBX SDK version " << mSdkManager->GetVersion() << std::endl;

			//Create an IOSettings object. This object holds all import/export settings.
			FbxIOSettings* ios = FbxIOSettings::Create(mSdkManager, IOSROOT);
			mSdkManager->SetIOSettings(ios);

			//Load plugins from the executable directory (optional)
			FbxString lPath = FbxGetApplicationDirectory();
			mSdkManager->LoadPluginsDirectory(lPath.Buffer());

			//Create an FBX scene. This object holds most objects imported/exported from/to files.
			mScene = FbxScene::Create(mSdkManager, _filePath.c_str());
			if (!mScene)
			{
				_status = "Error: Unable to create FBX scene!\n";
				return false;
			}

			mImporter = FbxImporter::Create(mSdkManager, "AGE FBX IMPORTER");
			if (!mImporter)
			{
				_status = "Error: Unable to create an FBX importer!\n";
				return false;
			}


			int lFileFormat = -1;
			if (!mSdkManager->GetIOPluginRegistry()->DetectReaderFileFormat(_filePath.c_str(), lFileFormat))
			{
				// Unrecognizable file format. Try to fall back to FbxImporter::eFBX_BINARY
				lFileFormat = mSdkManager->GetIOPluginRegistry()->FindReaderIDByDescription("FBX binary (*.fbx)");;
			}

			// Initialize the importer by providing a filename.
			if (mImporter->Initialize(_filePath.c_str(), lFileFormat) == true)
			{
				// The file is going to be imported at
				// the end of the first display callback.
			}
			else
			{
				_status = "Importer failed to import file\n";
				return false;
			}

			if (mImporter->Import(mScene) == true)
			{
				// Convert Axis System to what is used in this example, if needed
				FbxAxisSystem SceneAxisSystem = mScene->GetGlobalSettings().GetAxisSystem();
				FbxAxisSystem OurAxisSystem(FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eRightHanded);
				if (SceneAxisSystem != OurAxisSystem)
				{
					OurAxisSystem.ConvertScene(mScene);
				}

				// Convert Unit System to what is used in this example, if needed
				//FbxSystemUnit SceneSystemUnit = mScene->GetGlobalSettings().GetSystemUnit();
				//if (SceneSystemUnit.GetScaleFactor() != 1.0)
				//{
				//	//The unit in this example is centimeter.
				//	FbxSystemUnit::cm.ConvertScene(mScene);
				//}

				// Get the list of all the animation stack.
				mScene->FillAnimStackNameArray(mAnimStackNameArray);

				// Get the list of all the cameras in the scene.
				FillCameraArray(mScene, mCameraArray);

				// Convert mesh, NURBS and patch into triangle mesh
				FbxGeometryConverter lGeomConverter(mSdkManager);
				lGeomConverter.Triangulate(mScene, /*replace*/true);

				// Split meshes per material, so that we only have one material per mesh (for VBO support)
				lGeomConverter.SplitMeshesPerMaterial(mScene, /*replace*/true);

				// Bake the scene for one frame
				LoadCacheRecursive(mScene, mCurrentAnimLayer, _filePath.c_str());

				//// Convert any .PC2 point cache data into the .MC format for 
				//// vertex cache deformer playback.
				//PreparePointCacheData(mScene, mCache_Start, mCache_Stop);

				//// Get the list of pose in the scene
				//FillPoseArray(mScene, mPoseArray);

				return true;
			}
			return false;
		}


		void FillCameraArray(FbxScene* pScene, FbxArray<FbxNode*>& pCameraArray)
		{
			pCameraArray.Clear();

			FillCameraArrayRecursive(pScene->GetRootNode(), pCameraArray);
		}

		// Find all the cameras under this node recursively.
		void FillCameraArrayRecursive(FbxNode* pNode, FbxArray<FbxNode*>& pCameraArray)
		{
			if (pNode)
			{
				if (pNode->GetNodeAttribute())
				{
					if (pNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eCamera)
					{
						pCameraArray.Add(pNode);
					}
				}

				const int lCount = pNode->GetChildCount();
				for (int i = 0; i < lCount; i++)
				{
					FillCameraArrayRecursive(pNode->GetChild(i), pCameraArray);
				}
			}
		}

		// Bake node attributes and materials under this node recursively.
		// Currently only mesh, light and material.
		void LoadCacheRecursive(FbxNode * pNode, FbxAnimLayer * pAnimLayer)
		{
			//// Bake material and hook as user data.
			//const int lMaterialCount = pNode->GetMaterialCount();
			//for (int lMaterialIndex = 0; lMaterialIndex < lMaterialCount; ++lMaterialIndex)
			//{
			//    FbxSurfaceMaterial * lMaterial = pNode->GetMaterial(lMaterialIndex);
			//    if (lMaterial && !lMaterial->GetUserDataPtr())
			//    {
			//        FbxAutoPtr<MaterialCache> lMaterialCache(new MaterialCache);
			//        if (lMaterialCache->Initialize(lMaterial))
			//        {
			//            lMaterial->SetUserDataPtr(lMaterialCache.Release());
			//        }
			//    }
			//}

			FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();
			if (lNodeAttribute)
			{
				// Bake mesh as VBO(vertex buffer object) into GPU.
				if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
				{
					FbxMesh * lMesh = pNode->GetMesh();
					if (lMesh && !lMesh->GetUserDataPtr())
					{
						FbxAutoPtr<AGE::FBXMesh> lMeshCache(new AGE::FBXMesh);
						if (lMeshCache->Initialize(lMesh))
						{
							lMesh->SetUserDataPtr(lMeshCache.Release());
						}
					}
				}
				// Bake light properties.
				//else if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eLight)
				//{
				//	    FbxLight * lLight = pNode->GetLight();
				//	    if (lLight && !lLight->GetUserDataPtr())
				//	    {
				//	        FbxAutoPtr<LightCache> lLightCache(new LightCache);
				//	        if (lLightCache->Initialize(lLight, pAnimLayer))
				//	        {
				//	            lLight->SetUserDataPtr(lLightCache.Release());
				//	        }
				//	    }
				//	}
				//}

				const int lChildCount = pNode->GetChildCount();
				for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
				{
					LoadCacheRecursive(pNode->GetChild(lChildIndex), pAnimLayer);
				}
			}
		}

		// Bake node attributes and materials for this scene and load the textures.
		void LoadCacheRecursive(FbxScene * pScene, FbxAnimLayer * pAnimLayer, const char * pFbxFileName)
		{
			//// Load the textures into GPU, only for file texture now
			//const int lTextureCount = pScene->GetTextureCount();
			//for (int lTextureIndex = 0; lTextureIndex < lTextureCount; ++lTextureIndex)
			//{
			//    FbxTexture * lTexture = pScene->GetTexture(lTextureIndex);
			//    FbxFileTexture * lFileTexture = FbxCast<FbxFileTexture>(lTexture);
			//    if (lFileTexture && !lFileTexture->GetUserDataPtr())
			//    {
			//        // Try to load the texture from absolute path
			//        const FbxString lFileName = lFileTexture->GetFileName();
			//        
			//        // Only TGA textures are supported now.
			//        if (lFileName.Right(3).Upper() != "TGA")
			//        {
			//            FBXSDK_printf("Only TGA textures are supported now: %s\n", lFileName.Buffer());
			//            continue;
			//        }

			//        GLuint lTextureObject = 0;
			//        bool lStatus = LoadTextureFromFile(lFileName, lTextureObject);

			//        const FbxString lAbsFbxFileName = FbxPathUtils::Resolve(pFbxFileName);
			//        const FbxString lAbsFolderName = FbxPathUtils::GetFolderName(lAbsFbxFileName);
			//        if (!lStatus)
			//        {
			//            // Load texture from relative file name (relative to FBX file)
			//            const FbxString lResolvedFileName = FbxPathUtils::Bind(lAbsFolderName, lFileTexture->GetRelativeFileName());
			//            lStatus = LoadTextureFromFile(lResolvedFileName, lTextureObject);
			//        }

			//        if (!lStatus)
			//        {
			//            // Load texture from file name only (relative to FBX file)
			//            const FbxString lTextureFileName = FbxPathUtils::GetFileName(lFileName);
			//            const FbxString lResolvedFileName = FbxPathUtils::Bind(lAbsFolderName, lTextureFileName);
			//            lStatus = LoadTextureFromFile(lResolvedFileName, lTextureObject);
			//        }

			//        if (!lStatus)
			//        {
			//            FBXSDK_printf("Failed to load texture file: %s\n", lFileName.Buffer());
			//            continue;
			//        }

			//        if (lStatus)
			//        {
			//            GLuint * lTextureName = new GLuint(lTextureObject);
			//            lFileTexture->SetUserDataPtr(lTextureName);
			//        }
			//    }
			//}
			LoadCacheRecursive(pScene->GetRootNode(), pAnimLayer);
		}

	private:
		const std::string _filePath;
		std::string _status;
		FbxManager * mSdkManager;
		FbxScene * mScene;
		FbxImporter * mImporter;
		FbxAnimLayer * mCurrentAnimLayer;
		FbxNode * mSelectedNode;

		int mPoseIndex;
		FbxArray<FbxString*> mAnimStackNameArray;
		FbxArray<FbxNode*> mCameraArray;
		FbxArray<FbxPose*> mPoseArray;
	};
}