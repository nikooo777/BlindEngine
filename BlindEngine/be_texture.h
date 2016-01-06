#pragma once
#include "be_includes.h"
#include "be_object.h"

class BEtexture : public BEobject
{
public:
	BEtexture(std::string name, char* texture_path, char* texture_name);
	~BEtexture();
	void Render(glm::mat4) override;
	int texture_id_;
	FIBITMAP * texture_image_;
	void LoadTexture(char* path, char* name);
private:
};