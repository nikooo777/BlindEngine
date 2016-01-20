#include "be_texture.h"


BEtexture::BEtexture(std::string name, std::string texture_path, std::string texture_name) : BEobject(name)
{
	LoadTexture(texture_path, texture_name);
}


BEtexture::~BEtexture()
{
	FreeImage_Unload(texture_image_);
}


void BEtexture::Render(glm::mat4 world_matrix)
{
	// Update texture content
	glBindTexture(GL_TEXTURE_2D, texture_id_);
	// Set circular coordinates:
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);

	// Set min/mag filters:
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

void BEtexture::LoadTexture(std::string path, std::string name)
{

	std::cout << "Loading a texture at path: " << path << "with name: " << name << std::endl;
	std::string full_path = path + name;

	// Load an image from file:
	glGenTextures(1, &texture_id_);
	glBindTexture(GL_TEXTURE_2D, texture_id_);
	FIBITMAP *bitmap_image = FreeImage_Load(FreeImage_GetFileType(full_path.c_str(), 0), full_path.c_str());
	if (!bitmap_image)
	{
		std::cout << "Failed to load texture: " << full_path << std::endl;
	}

	texture_image_ = FreeImage_ConvertTo32Bits(bitmap_image);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, FreeImage_GetWidth(texture_image_), FreeImage_GetHeight(texture_image_), GL_BGRA_EXT, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(texture_image_));

	FreeImage_Unload(bitmap_image);
}

