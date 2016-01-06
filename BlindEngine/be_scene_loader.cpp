#include "be_scene_loader.h"
#include "be_mesh.h"
#include <map>
#include "be_light.h"
#include "be_material.h"


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

	std::cout << std::endl << "**Root node: **" << scene_->mRootNode->mName.C_Str() << std::endl;

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

	std::cout << std::endl << "***********MATERIALS**************" << std::endl;
	for (unsigned int i = 0; i < scene_->mNumMaterials; i++)
	{
		aiString strMat;
		aiMaterial* tmp = scene_->mMaterials[i];
		tmp->Get(AI_MATKEY_NAME, strMat);
		std::cout << strMat.C_Str() << std::endl;

		std::map<std::string, GLuint> textureIdMap;

		/* scan scene's materials for textures */
		for (unsigned int m = 0; m < scene_->mNumMaterials; ++m)
		{
			int texIndex = 0;
			aiString path;  // filename

			aiReturn texFound = scene_->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
			while (texFound == AI_SUCCESS)
			{
				std::cout << "\tTexture: " << path.C_Str() << std::endl;
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
		std::cout << scene_->mMeshes[i]->mName.C_Str() << std::endl;
	}

	// End debug info
	//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////// 
	ParseMaterials();

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

	be_logging::log("************");
	std::cout << "   mName : " << this_node->mName.C_Str() << std::endl;
	std::cout << "   mChildren : " << this_node->mChildren << std::endl;
	std::cout << "   mNumChildren : " << this_node->mNumChildren << std::endl;
	std::cout << "   mNumMeshes : " << this_node->mNumMeshes << std::endl;
	std::cout << "   mParent : " << this_node->mParent << std::endl;
	std::cout << "   mTransformation : " << this_node->mTransformation[0][0] << std::endl;

	// If parent isn't null, search wich type of node it is
	if (parent != nullptr)
	{
		aiAnimation *tmp_animation;
		aiCamera *tmp_camera;
		aiLight *tmp_light;
		// aiMaterial *tmp_material; // NOT in SceneGraph !!
		aiMesh *tmp_mesh;
		// aiTexture *tmp_texture; // NOT in SceneGraph !!


		if (scene_->mNumMeshes > 0 && (tmp_mesh = FindMesh(this_node->mName)) != nullptr)
		{
			std::cout << "A mesh was found. Extracting..." << std::endl;
			std::string name = tmp_mesh->mName.C_Str();
			//glm::vec3 *faces = (glm::vec3*) malloc(sizeof(glm::vec3)*tmp_mesh->mNumVertices);
			glm::vec3 *vertices = (glm::vec3*) malloc(sizeof(glm::vec3)*tmp_mesh->mNumVertices);
			glm::vec3 *normals = (glm::vec3*) malloc(sizeof(glm::vec3)*tmp_mesh->mNumVertices);
			glm::vec2 *texture_coords = (glm::vec2*) malloc(sizeof(glm::vec2)*tmp_mesh->mNumVertices);
			BEmesh* mesh = nullptr;


			//BEmesh *mesh = new BEmesh();

			//// Faces
			//if (tmp_mesh->HasFaces()){
			//	unsigned int faceIndex = 0;

			//	for (unsigned int i = 0; i < tmp_mesh->mNumFaces; ++i, faceIndex += 3) {
			//		const aiFace* tmp_face = &tmp_mesh->mFaces[i];
			//		faces[i] = glm::vec3(tmp_face->mIndices[0], tmp_face->mIndices[1], tmp_face->mIndices[2]);
			//	}
			//	mesh->SetFaces(faces);
			//}

			// Vertices position and normals (they come in pair)
			//if (tmp_mesh->HasPositions()){
			//	mesh->SetVerticesCount(tmp_mesh->mNumVertices);

			for (unsigned int i = 0; i < tmp_mesh->mNumVertices; i++)
			{
				vertices[i] = glm::vec3(tmp_mesh->mVertices[i].x, tmp_mesh->mVertices[i].y, tmp_mesh->mVertices[i].z);
				normals[i] = glm::vec3(tmp_mesh->mNormals[i].x, tmp_mesh->mNormals[i].y, tmp_mesh->mNormals[i].z);
			}
			//mesh->SetVertices(vertex);
			//}

			// Normals position
			/*if (tmp_mesh->HasNormals())
			{

			for (unsigned int i = 0; i < tmp_mesh->mNumVertices; i += 3)
			{
			normals[i] = glm::vec3(tmp_mesh->mNormals[i].x, tmp_mesh->mNormals[i].y, tmp_mesh->mNormals[i].z);
			}
			mesh->SetNormals(normals);
			}*/

			// Texture Coords
			/*	if (tmp_mesh->HasTextureCoords(0))
			{*/

			for (unsigned int i = 0; i < tmp_mesh->mNumVertices; i++)
			{
				texture_coords[i] = glm::vec2(tmp_mesh->mTextureCoords[0][i].x, tmp_mesh->mTextureCoords[0][i].y);
			}
			//mesh->SetTextureCoords(texture_coords);
			//	}

			// to change
			BEmaterial *material = nullptr;
			node = new BEmesh(name, vertices, tmp_mesh->mNumVertices, normals, texture_coords, material);
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
			std::cout << "Something unknown in the tree was found." << std::endl;
			node = nullptr; // @Todo: Edit it -> Only for not crash
		}

		node->SetParent(parent);
	}
	// If parent is null, it's the root node of this scene
	else
	{
		// Create node and set name
		const std::string name = std::string(this_node->mName.C_Str());
		node = new BEnode(name,BEnode::ROOT);
		node->SetAsRoot();
	}

	// Convert aiMatrix into an OpenGL matrix:
	glm::mat4 tranformation;
	memcpy(&tranformation, &this_node->mTransformation, sizeof tranformation);
	tranformation = glm::transpose(tranformation);
	node->SetTransformation(tranformation);
	//////////////////////////////////////////////////////////////////////////

	std::cout << parent;
	node->PrintName();

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
	char tmp_string[255];
	strcpy_s(tmp_string, sizeof tmp_string, name.C_Str());
	strcat_s(tmp_string, sizeof tmp_string, "Mesh");

	aiString to_compare;
	to_compare.Set(tmp_string);

	for (unsigned int i = 0; i < scene_->mNumMeshes; i++)
	{
		if (scene_->mMeshes[i]->mName == to_compare)
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
	if (texture_index < 0 || texture_index >(scene_->mNumTextures - 1))
		return nullptr;

	return scene_->mTextures[texture_index];
}

BElight* BEsceneLoader::ExtractLight(aiLight * tmp_light)
{
	BElight *light = nullptr;

	glm::vec3 ambient = glm::vec3(tmp_light->mColorAmbient.r, tmp_light->mColorAmbient.g, tmp_light->mColorAmbient.b);
	glm::vec3 diffuse = glm::vec3(tmp_light->mColorDiffuse.r, tmp_light->mColorDiffuse.g, tmp_light->mColorDiffuse.b);
	glm::vec3 specular = glm::vec3(tmp_light->mColorSpecular.r, tmp_light->mColorSpecular.g, tmp_light->mColorSpecular.b);
	glm::vec3 position = glm::vec3(tmp_light->mPosition.x, tmp_light->mPosition.y, tmp_light->mPosition.z);
	glm::vec3 direction = glm::vec3(tmp_light->mDirection.x, tmp_light->mDirection.y, tmp_light->mDirection.z);
	std::string light_name = tmp_light->mName.C_Str();

	switch (tmp_light->mType)
	{
		//omnidirectional
	case aiLightSource_POINT:
		light = BElight::CreateOmnidirectionalLight(light_name, ambient, diffuse, specular, position);
		break;

		//spotlight
	case aiLightSource_SPOT:
		light = BElight::CreateSpotLight(light_name, ambient, diffuse, specular, position, direction, tmp_light->mAngleInnerCone);
		//light->SetAngleOuterCone(tmp_light->mAngleOuterCone);
		break;
		//directional
	case aiLightSource_DIRECTIONAL:
		light = BElight::CreateDirectionalLight(light_name, ambient, diffuse, specular, direction);
		break;
	}

	light->SetAttenuationConstant(tmp_light->mAttenuationConstant);
	light->SetAttenuationLinear(tmp_light->mAttenuationLinear);
	light->SetAttenuationQuadratic(tmp_light->mAttenuationQuadratic);
	return light;
}

void set_float4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}

void color4_to_float4(const aiColor4D *c, float f[4])
{
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}

// Pass the materials' list in scene_ and add into the class BElist
void BEsceneLoader::ParseMaterials()
{
	aiMaterial *material_container;
	for (unsigned int i = 0; i < scene_->mNumMaterials; i++)
	{
		material_container = scene_->mMaterials[i];



		glm::vec4 diffuse;
		aiColor4D material_diffuse;
		if (AI_SUCCESS == aiGetMaterialColor(material_container, AI_MATKEY_COLOR_DIFFUSE, &material_diffuse))
			memcpy(&diffuse, &material_diffuse, sizeof diffuse);
		
		glm::vec4 ambient;
		aiColor4D material_ambient;
		if (AI_SUCCESS == aiGetMaterialColor(material_container, AI_MATKEY_COLOR_AMBIENT, &material_ambient))
			memcpy(&ambient, &material_ambient, sizeof ambient);

		glm::vec4 specular;
		aiColor4D material_specular;
		if (AI_SUCCESS == aiGetMaterialColor(material_container, AI_MATKEY_COLOR_SPECULAR, &material_specular))
			memcpy(&specular, &material_specular, sizeof specular);

		glm::vec4 emission;
		aiColor4D material_emission;
		if (AI_SUCCESS == aiGetMaterialColor(material_container, AI_MATKEY_COLOR_EMISSIVE, &material_emission))
			memcpy(&emission, &material_emission, sizeof emission);

		float shininess = 0.0;
		unsigned int max;
		aiGetMaterialFloatArray(material_container, AI_MATKEY_SHININESS, &shininess, &max);


		/************************************************************************/
		/* To store:
		 * 
		 * - Diffuse
		 * - Ambient
		 * - Specular
		 * - Emission
		 * - Shininess 
		/************************************************************************/

		/************************************************************************/
		/* Add to BElist
		/************************************************************************/
	}
}
