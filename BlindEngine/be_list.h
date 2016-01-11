#pragma once
#include <vector>
#include "be_includes.h"
#include "be_mesh.h"
#include "be_light.h"
#include "be_camera.h"
#include "be_material.h"

typedef struct meshcontainer{
	BEmesh* mesh;
	glm::mat4 tranformation;
} MeshContainer;

class BElist
{
public:
	BElist();
	~BElist();
	void RenderAll();
	void RenderMeshes();
	void RenderLights();
	void RenderCameras();

	void AddMaterial(BEmaterial* material){ material_ordered_list_.push_back(material); }
	BEmaterial* GetMaterial(unsigned int index){ return material_ordered_list_[index]; }

	void AddMesh(BEmesh*mesh);
	BEmesh* GetMesh(unsigned int index){ return mesh_ordered_list_[index]; }
	glm::mat4 GetMeshTransformation(BEmesh* mesh){ return meshes_.at(mesh); }
	BEmesh* GetMeshByName(std::string name);

	void AddLight(BElight* light);

private:
	// Transformation
	std::map<BEmesh*, glm::mat4> meshes_;
	std::map<BElight*, glm::mat4> lights_;
	std::map<BEcamera*, glm::mat4> cameras_;
	
	// Utility
	std::vector<BEmaterial*> material_ordered_list_;
	std::vector<BEmesh*> mesh_ordered_list_;

	// Find utility
	std::map<std::string, BEmesh*> meshes_by_name_;
};