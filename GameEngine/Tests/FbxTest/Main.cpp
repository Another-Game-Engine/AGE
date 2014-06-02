#include <fbxsdk.h>
#include <string>
#include <cassert>
#include <iostream>

void ProcessSkeletonNode(FbxScene* fbxScene, FbxNode* node){
	FbxSkeleton* bone = FbxCast<FbxSkeleton>(node->GetNodeAttribute());
	std::string logname = LogNameForObject(node);
	std::ofstream log(logname.c_str());

	FbxDouble3 pos = node->LclTranslation;
	log << bone->GetName();
	log << "\tPosition:" << pos[0] <<' ' << pos[1] << ' '<< pos[2];
	log << "\tLength:" << bone->LimbLength;
	log << "\tSize: "<< bone->Size <<'\n';
	log << "parent bone: ";

	FbxNode* parent = node->GetParent();
	if (parent && NodeAttributeIsType(parent, FbxNodeAttribute::eSkeleton))
		log << parent->GetName() << '\n';
	else
		log << "null\n";

	WalkAnimations(node, fbxScene, log);

	log.close();
}

bool NodeAttributeIsType(FbxNode* node, FbxNodeAttribute::EType type)
{
	if (!node)
		return false;

	FbxNodeAttribute* attribute = node->GetNodeAttribute();

	if (!attribute)
		return false;

	return attribute->GetAttributeType() == type;
}

void loadNode(FbxScene* fbxScene, FbxNode* node, size_t depth)
{
	std::cout << node->GetName() << '(' << node->GetTypeName()<< ")\n";

	if (NodeAttributeIsType(node, FbxNodeAttribute::eSkeleton))
		ProcessSkeletonNode(fbxScene, node);

	int childCount = node->GetChildCount();

	for (int i = 0; i < childCount; i++)
		loadNode(fbxScene, node->GetChild(i), depth + 1);
}

void loadScene(FbxScene* fbxScene)
{
	FbxNode* root = fbxScene->GetRootNode();

	FbxAMatrix rootTransform;
	FbxDouble3 t = root->LclTranslation.Get();
    FbxDouble3 r =root->LclRotation.Get();
    FbxDouble3 s =root->LclScaling.Get();

	loadNode(fbxScene, root, 0);
}

void main(void)
{
	std::string path = "humanoid.fbx";

	FbxManager* fbxManager = FbxManager::Create();
	FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "FBXIMPORTER");
    FbxScene* fbxScene = FbxScene::Create(fbxManager , "FBXSCENE");

	bool result = fbxImporter->Initialize(path.c_str());
	assert(result);

	result = fbxImporter->Import(fbxScene);
	assert(result);

	if (result){
		loadScene(fbxScene);
	}
	else{
		std::cout << "Import failed!" << std::endl;
		return;
	}

	fbxScene->Destroy();
    fbxManager->Destroy();
}