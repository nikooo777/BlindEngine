#include "be_texture.h"


BEtexture::BEtexture(std::string name, std::string texture_path, std::string texture_name) : BEobject(name)
{
	LoadTexture(texture_path, texture_name);
}


BEtexture::~BEtexture()
{
	FreeImage_Unload(texture_image_);
}

void BEtexture::Render(glm::mat4 f)
{
	
	glEnable(GL_TEXTURE_2D);

	// Update texture content
	glBindTexture(GL_TEXTURE_2D, texture_id_);

	// Set circular coordinates:
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);

	// Set min/mag filters:
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, FreeImage_GetWidth(texture_image_), FreeImage_GetHeight(texture_image_), GL_BGRA_EXT, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(texture_image_));
	//glDisable(GL_TEXTURE_2D);
	std::cout << "Rendering texture: " << get_name() << std::endl;
	
}

void BEtexture::LoadTexture(std::string path, std::string name)
{
	std::string full_path = path + name;

	// Load an image from file:
	FIBITMAP *bitmap_image = FreeImage_Load(FreeImage_GetFileType(full_path.c_str(), 0), full_path.c_str());
	texture_image_ = FreeImage_ConvertTo32Bits(bitmap_image);
	FreeImage_Unload(bitmap_image);
}