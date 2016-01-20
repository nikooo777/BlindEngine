#include "be_scene_loader.h"
#include <map>
#include "be_light.h"
#include "be_material.h"
#include "be_engine.h"


BEsceneLoader::BEsceneLoader()
{
	cnt_materials_ = BEengine::lists_->GetMaterialCount();
	cnt_meshes_ = BEengine::lists_->GetMeshCount();
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
	std::cout << "loading scene located in:" << scene_path << std::endl;
	scene_ = (aiScene *)importer_.ReadFile(scene_path, aiProcess_Triangulate);
	if (scene_ == nullptr)
	{
		std::cout << "ERROR: unable to load scene!" << std::endl;
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

	std::cout << std::endl << "**Root node**: " << scene_->mRootNode->mName.C_Str() << std::endl;

	std::cout << std::endl << "***********ANIMATIONS**************" << std::endl;
	for (unsigned int i = 0; i < scene_->mNumAnimations; i++)
	{
		std::cout << "   " << scene_->mAnimations[i]->mName.C_Str() << std::endl;
	}

	std::cout << std::endl << "***********CAMERAS**************" << std::endl;
	for (unsigned int i = 0; i < scene_->mNumCameras; i++)
	{
		std::cout << "   " << scene_->mCameras[i]->mName.C_Str() << std::endl;
	}

	std::cout << std::endl << "***********LIGHTS**************" << std::endl;
	for (unsigned int i = 0; i < scene_->mNumLights; i++)
	{
		std::cout << "   " << scene_->mLights[i]->mName.C_Str() << std::endl;
	}

	std::cout << std::endl << "***********MATERIALS**************" << std::endl;
	for (unsigned int i = 0; i < scene_->mNumMaterials; i++)
	{
		aiString strMat;
		aiMaterial* tmp = scene_->mMaterials[i];
		tmp->Get(AI_MATKEY_NAME, strMat);
		std::cout << "   " << strMat.C_Str() << std::endl;

		std::map<std::string, GLuint> textureIdMap;

		/* scan scene's materials for textures */
		for (unsigned int m = 0; m < scene_->mNumMaterials; ++m)
		{
			int texIndex = 0;
			aiString path;  // filename

			aiReturn texFound = scene_->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
			while (texFound == AI_SUCCESS)
			{
				std::cout << "   " << "\tTexture: " << path.C_Str() << std::endl;
				//fill map with textures, OpenGL image ids set to 0
				textureIdMap[path.data] = 0;
				// more textures?
				texIndex++;
				texFound = scene_->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
			}
		}
	}

	std::cout << std::endl << "***********MESHES**************" << std::endl;
	for (unsigned int i = 0; i < scene_->mNumMeshes; i++)
	{
		std::cout << "   " << scene_->mMeshes[i]->mName.C_Str() << std::endl;
	}

	// End debug info
	//////////////////////////////////////////////////////////////////////////

	ParseMaterials();
	ParseMeshes();

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
	BEnode *node; // To change: Added only for compiling

	//std::cout << "   mName : " << this_node->mName.C_Str() << std::endl;
	//std::cout << "   mChildren : " << this_node->mChildren << std::endl;
	//std::cout << "   mNumChildren : " << this_node->mNumChildren << std::endl;
	//std::cout << "   mNumMeshes : " << this_node->mNumMeshes << std::endl;
	//std::cout << "   mParent : " << this_node->mParent << std::endl;
	//std::cout << "   mTransformation : " << this_node->mTransformation[0][0] << std::endl;

	// If parent isn't null, search wich type of node it is
	if (parent != nullptr)
	{
		aiAnimation *tmp_animation;
		aiCamera *tmp_camera;
		aiLight *tmp_light;
		BEmesh *tmp_mesh;


		if (this_node->mNumMeshes && (tmp_mesh = BEengine::lists_->GetMesh(this_node->mMeshes[0])))
		{
			//std::cout << "A mesh was found. Extracting..." << std::endl;

			tmp_mesh->set_name(this_node->mName.C_Str());
			tmp_mesh->SetSubMeshes(this_node->mNumMeshes, this_node->mMeshes, cnt_meshes_);
			BEengine::lists_->AddMeshToMap(tmp_mesh);
			node = tmp_mesh;
		}
		else if ((tmp_camera = FindCamera(this_node->mName)) != nullptr)
		{
			std::cout << "A camera was found. Extracting..." << std::endl;
			node = nullptr; // @Todo: Edit it -> Only for not crash
		}
		else if ((tmp_animation = FindAnimation(this_node->mName)) != nullptr)
		{
			std::cout << "An animation was found in the tree but it's not supported." << std::endl;
			node = nullptr; // @Todo: Edit it -> Only to avoid a crash
		}
		else if ((tmp_light = FindLight(this_node->mName)) != nullptr)
		{
			std::cout << "A light was found. Extracting..." << std::endl;
			node = ExtractLight(tmp_light);
		}
		else
		{
			std::cout << "NodeHelper possible --> Something unknown in the tree was found." << std::endl;
			const std::string name = std::string(this_node->mName.C_Str());
			node = new BEnode(name, BEnode::ROOT);
		}

		node->SetParent(parent);
	}
	// If parent is null, it's the root node of this scene
	else
	{
		// Create node and set name
		const std::string name = std::string(this_node->mName.C_Str());
		node = new BEnode(name, BEnode::ROOT);
		node->SetAsSceneRoot();
	}

	// Convert aiMatrix into an OpenGL matrix:
	glm::mat4 tranformation;
	memcpy(&tranformation, &this_node->mTransformation, sizeof tranformation);
	tranformation = glm::transpose(tranformation);
	node->SetTransformation(tranformation);
	//////////////////////////////////////////////////////////////////////////

	for (unsigned int i = 0; i < this_node->mNumChildren; i++)
	{
		node->AddChild(BuildScene(root, node, this_node->mChildren[i]));
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
	std::string target = std::string(name.C_Str());

	for (unsigned int i = 0; i < scene_->mNumLights; i++)
	{
		if (scene_->mLights[i]->mName == name || target.find(".Target") != std::string::npos)
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
* Find a texture in textures' array from scene.
* @param texture_index Texture's index taken from ???
* @return Texture if exists
*/
aiTexture* BEsceneLoader::FindTexture(unsigned int texture_index)
{
	if (texture_index < 0 || texture_index >(scene_->mNumTextures - 1))
		return nullptr;

	return scene_->mTextures[texture_index];
}

/************************************************************************/
/* Light
/************************************************************************/
BElight* BEsceneLoader::ExtractLight(aiLight * light_container)
{
	BElight *light = nullptr;

	glm::vec3 ambient = glm::vec3(light_container->mColorAmbient.r, light_container->mColorAmbient.g, light_container->mColorAmbient.b);
	glm::vec3 diffuse = glm::vec3(light_container->mColorDiffuse.r, light_container->mColorDiffuse.g, light_container->mColorDiffuse.b);
	glm::vec3 specular = glm::vec3(light_container->mColorSpecular.r, light_container->mColorSpecular.g, light_container->mColorSpecular.b);
	glm::vec3 position = glm::vec3(light_container->mPosition.x, light_container->mPosition.y, light_container->mPosition.z);
	glm::vec3 direction = glm::vec3(light_container->mDirection.x, light_container->mDirection.y, light_container->mDirection.z);
	std::string light_name = light_container->mName.C_Str();

	switch (light_container->mType)
	{
		//omnidirectional
	case aiLightSource_POINT:
		light = BElight::CreateOmnidirectionalLight(light_name, ambient, diffuse, specular, position);
		break;

		//spotlight
	case aiLightSource_SPOT:
		light = BElight::CreateSpotLight(light_name, ambient, diffuse, specular, position, direction, glm::degrees(light_container->mAngleInnerCone));
		//light->SetAngleOuterCone(tmp_light->mAngleOuterCone);
		break;
		//directional
	case aiLightSource_DIRECTIONAL:
		light = BElight::CreateDirectionalLight(light_name, ambient, diffuse, specular, direction);
		break;
	}

	light->SetAttenuationConstant(light_container->mAttenuationConstant);
	light->SetAttenuationLinear(light_container->mAttenuationLinear);
	light->SetAttenuationQuadratic(light_container->mAttenuationQuadratic);

	BEengine::lists_->AddLight(light);

	return light;
}

/************************************************************************/
/* Mesh
/************************************************************************/
void BEsceneLoader::ParseMeshes()
{
	for (unsigned int i = 0; i < scene_->mNumMeshes; i++)
	{
		aiMesh* mesh_container = scene_->mMeshes[i];

		std::string name = mesh_container->mName.C_Str();
		name = name.substr(0, name.length() - 4);

		glm::vec3 *vertices = (glm::vec3*) malloc(sizeof(glm::vec3)*mesh_container->mNumVertices);
		glm::vec3 *normals = (glm::vec3*) malloc(sizeof(glm::vec3)*mesh_container->mNumVertices);
		glm::vec2 *texture_coords = (glm::vec2*) malloc(sizeof(glm::vec2)*mesh_container->mNumVertices);

		for (unsigned int i = 0; i < mesh_container->mNumVertices; i++)
		{
			vertices[i] = glm::vec3(mesh_container->mVertices[i].x, mesh_container->mVertices[i].y, mesh_container->mVertices[i].z);
			normals[i] = glm::vec3(mesh_container->mNormals[i].x, mesh_container->mNormals[i].y, mesh_container->mNormals[i].z);
		}


		if (mesh_container->mTextureCoords && mesh_container->mTextureCoords[0])
			for (unsigned int i = 0; i < mesh_container->mNumVertices; i++)
				texture_coords[i] = glm::vec2(mesh_container->mTextureCoords[0][i].x, mesh_container->mTextureCoords[0][i].y);

		// Material
		BEmaterial *material = nullptr;
		material = BEengine::lists_->GetMaterial(mesh_container->mMaterialIndex + cnt_materials_);

		//std::cout << "Mesh parsed: " << name << std::endl;
		BEengine::lists_->AddMesh(new BEmesh(name, vertices, mesh_container->mNumVertices, normals, texture_coords, material, 0, nullptr));
	}
}

/************************************************************************/
/* Material
/************************************************************************/
// Pass the materials' list in scene_ and add into the class BElist
void BEsceneLoader::ParseMaterials()
{
	aiMaterial *material_container;
	std::cout << "************" << std::endl;
	std::cout << std::endl << "NumMaterial: " << scene_->mNumMaterials << std::endl;
	for (unsigned int i = 0; i < scene_->mNumMaterials; i++)
	{
		material_container = scene_->mMaterials[i];

		aiString matName;
		material_container->Get(AI_MATKEY_NAME, matName);
		std::cout << "    MatName    : " << matName.C_Str() << std::endl;

		// Get properties:
		glm::vec4 ambient;
		aiColor4D material_ambient;
		material_container->Get(AI_MATKEY_COLOR_AMBIENT, material_ambient);
		memcpy(&ambient, &material_ambient, sizeof ambient);
		//std::cout << "    Ambient : " << material_ambient.r << ", " << material_ambient.g << ", " << material_ambient.b << ", " << material_ambient.a << std::endl;

		glm::vec4 diffuse;
		aiColor4D material_diffuse;
		material_container->Get(AI_MATKEY_COLOR_DIFFUSE, material_diffuse);
		memcpy(&diffuse, &material_diffuse, sizeof diffuse);
		//std::cout << "    Diffuse : " << material_diffuse.r << ", " << material_diffuse.g << ", " << material_diffuse.b << ", " << material_diffuse.a << std::endl;

		glm::vec4 specular;
		aiColor4D material_specular;
		material_container->Get(AI_MATKEY_COLOR_SPECULAR, material_specular);
		memcpy(&specular, &material_specular, sizeof specular);
		//std::cout << "    Specular: " << material_specular.r << ", " << material_specular.g << ", " << material_specular.b << ", " << material_specular.a << std::endl;
		float material_opacity;
		material_container->Get(AI_MATKEY_OPACITY, material_opacity);

		float shininess, shininess_strength;
		material_container->Get(AI_MATKEY_SHININESS, shininess);
		material_container->Get(AI_MATKEY_SHININESS_STRENGTH, shininess_strength);
		//std::cout << "    Shinin. : " << shininess << " " << shininess_strength << std::endl;

		BEtexture *texture = nullptr;
		if (material_container->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString textureName;
			material_container->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), textureName);
			std::cout << "    Texture : " << textureName.C_Str() << std::endl;
			texture = new BEtexture(textureName.C_Str(), "", textureName.C_Str()); //is the path included in the name?
		}

		BEmaterial *material = new BEmaterial(matName.C_Str(), ambient, diffuse, specular, material_opacity, shininess, shininess_strength, texture);
		BEengine::lists_->AddMaterial(material);
	}
}

