#pragma once
#include <vector>
#include "be_includes.h"
#include "be_mesh.h"
#include "be_light.h"
#include "be_camera.h"
#include "be_material.h"

class BElist
{
public:
	BElist();
	~BElist();
	void RenderAll();
	void RenderMeshes();
	void RenderLights();
	void RenderCameras();

	void AddMaterial(BEmaterial* material){ materials_.push_back(material); }
	BEmaterial* GetMaterial(unsigned int index){ return materials_[index]; }

	void AddMesh(BEmesh*mesh){ meshes_.push_back(mesh); }
	BEmesh* GetMesh(unsigned int index){ return meshes_[index]; }

private:
	//std::map<BEmesh*, glm::mat4> meshes_;
	std::map<BElight*, glm::mat4> lights_;
	std::map<BEcamera*, glm::mat4> cameras_;
	
	std::vector<BEmaterial*> materials_;
	std::vector<BEmesh*> meshes_;
};