#pragma once
// DLL
#include "be_dll_interface.h"

#include "be_includes.h"
#include "be_mesh.h"
#include "be_light.h"
#include "be_camera.h"
#include "be_material.h"

class LIB_API BElist
{
public:
	BElist();
	~BElist();
	void RenderAll();
	void RenderMeshes();
	void RenderLights();
	void RenderCameras();

	// Material
	void AddMaterial(BEmaterial* material){ material_ordered_list_.push_back(material); }
	BEmaterial* GetMaterial(unsigned int index){ return material_ordered_list_[index]; }
	unsigned int GetMaterialCount(){ return material_ordered_list_.size(); }

	// Mesh
	void AddMesh(BEmesh*mesh);
	void AddMesh(BEmesh*mesh, glm::mat4 f);
	void UpdateMesh(BEmesh*mesh, glm::mat4 f);
	BEmesh* GetMesh(unsigned int index){ return mesh_ordered_references_[index]; }
	glm::mat4 GetMeshTransformation(BEmesh* mesh){ return meshes_.at(mesh); }
	BEmesh* GetMeshByName(std::string name);
	unsigned int GetMeshCount(){ return meshes_.size(); }

	// Light
	void AddLight(BElight* light);
	void AddLight(BElight* light, glm::mat4 f);
	void UpdateLight(BElight* light, glm::mat4 f);

	// Camera
	void AddCamera(BEcamera* camera);
	void AddCamera(BEcamera* camera, glm::mat4 f);
	void UpdateCamera(BEcamera* camera, glm::mat4 f);

private:
	// Transformation
	std::map<BEmesh*, glm::mat4> meshes_;
	std::map<BElight*, glm::mat4> lights_;
	std::map<BEcamera*, glm::mat4> cameras_;
	
	// Utility
	std::vector<BEmaterial*> material_ordered_list_;
	std::vector<BEmesh*> mesh_ordered_references_;

	// Find utility
	std::map<std::string, BEmesh*> meshes_by_name_;
};
