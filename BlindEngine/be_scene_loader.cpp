#include "be_scene_loader.h"
#include "be_mesh.h"


BEsceneLoader::BEsceneLoader()
{
}


BEsceneLoader::~BEsceneLoader()
{
}

/**
* Build the scene graph and return the root
* @param scene_path Name of file to load
* @return Root from scene
*/
BEnode*  BEsceneLoader::LoadScene(char * scene_path)
{
	be_logging::log("loading scene located in:");
	be_logging::log(scene_path);
	scene_ = (aiScene *)importer_.ReadFile(scene_path, aiProcess_Triangulate);
	if (scene_ == nullptr)
	{
		be_logging::log("ERROR: unable to load scene!");
		return nullptr;
	}

	// Scene summary:
	std::cout << std::endl << "Scene summary: " << std::endl;
	std::cout << "   Meshes  : " << scene_->mNumMeshes << std::endl;
	std::cout << "   Lights   : " << scene_->mNumLights << std::endl;
	std::cout << "   Materials: " << scene_->mNumMaterials << std::endl;
	std::cout << "   Anims    : " << scene_->mNumAnimations << std::endl;
	std::cout << "   Cameras  : " << scene_->mNumCameras << std::endl;
	std::cout << "   Textures : " << scene_->mNumTextures << std::endl;
	
	//////////////////////////////////////////////////////////////////////////
	// Debug info
	
	std::cout << std::endl << "**Root node: **"<< scene_->mRootNode->mName.C_Str() << std::endl;
	
	std::cout << std::endl << "***********ANIMATIONS**************" << std::endl;
	for (unsigned int i = 0; i < scene_->mNumAnimations; i++)
	{
		std::cout << scene_->mAnimations[i]->mName.C_Str() << std::endl;
	}

	std::cout << std::endl << "***********CAMERAS**************" << std::endl;
	for (unsigned int i = 0; i < scene_->mNumCameras; i++)
	{
		std::cout << scene_->mCameras[i]->mName.C_Str() << std::endl;
	}

	std::cout << std::endl << "***********LIGHTS**************" << std::endl;
	for (unsigned int i = 0; i < scene_->mNumLights; i++)
	{
		std::cout << scene_->mLights[i]->mName.C_Str() << std::endl;
	}

	std::cout << std::endl << "***********MESHES**************" << std::endl;
	for (unsigned int i = 0; i < scene_->mNumMeshes; i++)
	{
		std::cout << scene_->mMeshes[i]->mName.C_Str() << std::endl;
	}

	// End debug info
	//////////////////////////////////////////////////////////////////////////

	return BuildScene(scene_->mRootNode, nullptr, scene_->mRootNode);
}


/**
* Build the scene graph and return the root
* @param root Name of the light to search
* @param parent Name of the light to search
* @param this_node Name of the light to search
* @return Root from scene
*/
BEnode* BEsceneLoader::BuildScene(aiNode* root, BEnode* parent, aiNode* this_node)
{
	BEnode *node = nullptr;

	be_logging::log("************");
	std::cout << "   mName : " << this_node->mName.C_Str() << std::endl;
	std::cout << "   mChildren : " << this_node->mChildren << std::endl;
	std::cout << "   mNumChildren : " << this_node->mNumChildren << std::endl;
	std::cout << "   mNumMeshes : " << this_node->mNumMeshes << std::endl;
	std::cout << "   mParent : " << this_node->mParent << std::endl;
	std::cout << "   mTransformation : " << this_node->mTransformation[0][0] << std::endl;

	aiAnimation *tmp_animation;
	aiCamera *tmp_camera;
	aiLight *tmp_light;
	aiMaterial *tmp_material;
	aiMesh *tmp_mesh;
	aiTexture *tmp_texture;


	if (scene_->mNumMeshes > 0 && (tmp_mesh = FindMesh(this_node->mName)) != nullptr)
	{
		std::cout << "It's a mesh" << std::endl;
	}
	else if ((tmp_camera = FindCamera(this_node->mName)) != nullptr)
	{
		std::cout << "It's a camera" << std::endl;
	}
	else if ((tmp_animation = FindAnimation(this_node->mName)) != nullptr)
	{
		std::cout << "It's an animation" << std::endl;
	}
	else if ((tmp_light = FindLight(this_node->mName)) != nullptr)
	{
		std::cout << "It's a Light" << std::endl;
	}
	else
	{
		std::cout << "I don't know what's that" << std::endl;
	}



	for (unsigned int i = 0; i < this_node->mNumChildren; i++)
	{
		BuildScene(root, node, this_node->mChildren[i]);
	}
	return node;
}


/**
* Find an animation in animations' array from scene.
* @param name Name of the animation to search
* @return Animation if found
*/
aiAnimation* BEsceneLoader::FindAnimation(aiString name)
{

	for (unsigned int i = 0; i < scene_->mNumAnimations; i++)
	{
		if (scene_->mAnimations[i]->mName == name)
			return scene_->mAnimations[i];
	}

	return nullptr;
}

/**
* Find a camera in cameras' array from scene.
* @param name Name of the camera to search
* @return Camera if found
*/
aiCamera* BEsceneLoader::FindCamera(aiString name)
{
	for (unsigned int i = 0; i < scene_->mNumCameras; i++)
	{
		if (scene_->mCameras[i]->mName == name)
			return scene_->mCameras[i];
	}

	return nullptr;
}

/**
* Find a light in lights' array from scene.
* @param name Name of the light to search
* @return Light if found
*/
aiLight* BEsceneLoader::FindLight(aiString name)
{
	for (unsigned int i = 0; i < scene_->mNumLights; i++)
	{
		if (scene_->mLights[i]->mName == name)
			return scene_->mLights[i];
	}

	return nullptr;
}

/**
* Find a material in materials' array from scene.
* @param mMaterialIndex Material's index taken from Mesh
* @return Material if exists
*/
aiMaterial* BEsceneLoader::FindMaterial(unsigned int mMaterialIndex)
{
	if (mMaterialIndex<0 || mMaterialIndex >(scene_->mNumMaterials - 1))
		return nullptr;

	return scene_->mMaterials[mMaterialIndex];
}

/**
* Find a mesh in meshes' array from scene.
* @param name Name of the mesh to search
* @return Mesh if found
*/
aiMesh* BEsceneLoader::FindMesh(aiString name)
{
	for (unsigned int i = 0; i < scene_->mNumMeshes; i++)
	{
		if (scene_->mMeshes[i]->mName == name)
			return scene_->mMeshes[i];
	}

	return nullptr;
}

/**
* Find a texture in textures' array from scene.
* @param texture_index Texture's index taken from ???
* @return Texture if exists
*/
aiTexture* BEsceneLoader::FindTexture(unsigned int texture_index)
{
	if (texture_index<0 || texture_index >(scene_->mNumTextures - 1))
		return nullptr;

	return scene_->mTextures[texture_index];
}
