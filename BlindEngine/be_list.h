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
	void AddMaterial(BEmaterial* material);
	BEmaterial* GetMaterial(unsigned int index);
	unsigned int GetMaterialCount();

	// Mesh
	void AddMesh(BEmesh*mesh);
	void AddMesh(BEmesh*mesh, glm::mat4 f);
	void AddMeshToMap(BEmesh*mesh);
	void UpdateMesh(BEmesh*mesh, glm::mat4 f);
	BEmesh* GetMesh(unsigned int index);
	//glm::mat4 GetMeshTransformation(BEmesh* mesh){ return meshes_.at(mesh); }
	BEmesh* GetMeshByName(std::string name);
	unsigned int GetMeshCount();

	// Light
	void AddLight(BElight* light);
	void AddLight(BElight* light, glm::mat4 f);
	void UpdateLight(BElight* light, glm::mat4 f);

	// Camera
	void AddCamera(BEcamera* camera);
	void AddCamera(BEcamera* camera, glm::mat4 f);
	void UpdateCamera(BEcamera* camera, glm::mat4 f);
	void DeepSort();
private:
	typedef struct Mesh
	{
		BEmesh* mesh_;
		glm::mat4 world_coords_;
	}Mesh;

	// Transformation
	//std::map<BEmesh*, glm::mat4> meshes_;
	std::map<BElight*, glm::mat4> lights_;
	std::map<BEcamera*, glm::mat4> cameras_;

	std::vector<Mesh*> meshes_v_;

	// Utility
	std::vector<BEmaterial*> material_ordered_list_;
	std::vector<BEmesh*> mesh_ordered_references_;

	// Find utility
	std::map<std::string, BEmesh*> meshes_by_name_;
};
