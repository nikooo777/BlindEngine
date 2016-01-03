#pragma once
#include "be_node.h"
#include "be_logging.h"
#include "be_includes.h"

class BEsceneLoader
{
public:
	BEsceneLoader();
	~BEsceneLoader();

	BEnode * LoadScene(char *);
private:
	Assimp::Importer importer_;
	aiScene *scene_;
	BEnode * BuildScene(aiNode *);
};

