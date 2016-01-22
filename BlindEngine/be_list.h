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

	// Stencil
	void EnableStencilFiltering();
	void RenderTransparentMeshes();
	void SetupStencil();

	// Render
	void RenderAll();
	void RenderMeshes();
	void RenderOpaqueMeshes();
	void RenderReflectedMeshes();
	void RenderLights();
	//void RenderCameras();
	void RenderMirrored();
	void RenderShadows();

	// Mirrored
	void PassMirrored(BEmesh*mesh, glm::mat4 world_coords);
	void AddMirrored(BEmesh*mesh);

	// Shadows
	void RemoveShadows(BEmesh*mesh);
	void PassShadowed(BEmesh*mesh, glm::mat4 world_coords);

	// Material
	void AddMaterial(BEmaterial* material);
	BEmaterial* GetMaterial(unsigned int index);
	unsigned int GetMaterialCount();

	// Mesh
	void AddMesh(BEmesh*mesh);

	void Pass(BEmesh*mesh, glm::mat4 world_coords);
	BEmesh* GetMesh(unsigned int index);
	unsigned int GetMeshCount();

	// Light
	void AddLight(BElight* light);
	void Pass(BElight* light, glm::mat4 world_coords);

	// Camera
	/*void AddCamera(BEcamera* camera);
	void Pass(BEcamera* camera, glm::mat4 world_coords);*/

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
	std::vector<Mesh*> mirrored_v_;
	std::vector<Mesh*> shadows_v_;

	// Utility
	std::vector<BEmaterial*> material_ordered_list_;
	std::vector<BEmesh*> mesh_ordered_references_;
};
