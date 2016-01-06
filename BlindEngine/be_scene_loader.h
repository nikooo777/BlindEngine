#pragma once
#include "be_node.h"
#include "be_light.h"
#include "be_logging.h"
#include "be_includes.h"

class BEsceneLoader
{
public:
	BEsceneLoader();
	~BEsceneLoader();

	BEnode* LoadScene(char *);
private:
	Assimp::Importer importer_;
	aiScene* scene_;

	BEnode* BuildScene(aiNode* root, BEnode* parent, aiNode* thisNode);

	BEnode * ExtractMesh(aiMesh * tmp_mesh);

	BElight* ExtractLight(aiLight* tmp_light);

	/*******************************
	*	Find functions
	*/
	aiAnimation* FindAnimation(aiString name);
	aiCamera* FindCamera(aiString name);
	aiLight* FindLight(aiString name);
	aiMaterial* FindMaterial(unsigned int mMaterialIndex);
	aiMesh* FindMesh(aiString name);
	aiTexture* FindTexture(unsigned int texture_index);
};