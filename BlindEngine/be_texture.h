#pragma once
// DLL
#include "be_dll_interface.h"

#include "be_includes.h"
#include "be_object.h"

class LIB_API BEtexture : public BEobject
{
public:
	BEtexture(std::string name, std::string texture_path, std::string texture_name);
	~BEtexture();
	//void Render(glm::mat4 world_matrix) override;
	virtual void Render(glm::mat4 world_matrix) override;

	unsigned int texture_id_;
	FIBITMAP * texture_image_;
	void LoadTexture(std::string path, std::string name);
private:
};