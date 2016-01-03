#pragma once
#include "be_node.h"
#include "be_logging.h"
//#include "FreeImage.h"
// Assimp:
#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>
#include <version.h>

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

