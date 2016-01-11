#include "be_light.h"
#include "be_engine.h"

//initialization of the counter
int BElight::total_lights = 0;

//global constructor
BElight::BElight(const LightType type, const std::string name, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, glm::vec3 position, glm::vec3 direction, float cutoff) : BEnode(name, LIGHT)
{
	if (total_lights <= 7)
	{
		light_number_ = (total_lights++) + GL_LIGHT0;
		glEnable(light_number_);
	}
	else
		throw new std::runtime_error("There are too many lights in the scene!");

	ambient_ = ambient;
	diffuse_ = diffuse;
	specular_ = specular;
	type_ = type;
	cutoff_ = cutoff;

	switch (type)
	{
	case DIRECTIONAL:
		//slide 37 of cg_opengl.pdf
		position_ = glm::vec4(direction, 0.f);  //the direction is meant to be hardcoded in the position with w=0
		break;

	case OMNIDIRECTIONAL:
		//slide 38 of cg_opengl.pdf
		position_ = glm::vec4(position, 1.f);
		break;

	case SPOTLIGHT:
		//slide 39 of cg_opengl.pdf
		position_ = glm::vec4(position, 1.f);;
		break;
	}
}

BElight* BElight::CreateOmnidirectionalLight(const std::string name, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position)
{
	return new BElight(OMNIDIRECTIONAL, name, glm::vec4(ambient, 1.0f), glm::vec4(diffuse, 1.0f), glm::vec4(specular, 1.0f), position, glm::vec3(), 0);
}

BElight* BElight::CreateDirectionalLight(const std::string name, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction)
{
	return new BElight(DIRECTIONAL, name, glm::vec4(ambient, 1.0f), glm::vec4(diffuse, 1.0f), glm::vec4(specular, 1.0f), glm::vec3(), direction, 180.f);
}

BElight* BElight::CreateSpotLight(const std::string name, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, glm::vec3 direction, float cutoff)
{
	return new BElight(SPOTLIGHT, name, glm::vec4(ambient, 1.0f), glm::vec4(diffuse, 1.0f), glm::vec4(specular, 1.0f), position, direction, cutoff);
}

void BElight::Render(glm::mat4 cumulated_transformation_matrix)
{
	std::cout << "Rendering a Light: " << get_name() << "number: "<< light_number_ <<std::endl;
	glm::mat4 tmp_f = cumulated_transformation_matrix*transformation_;
	glLoadMatrixf(glm::value_ptr(tmp_f));

	BEengine::lists_->UpdateLight(this, tmp_f);


	//Common color property
	/*glLightfv(light_number_, GL_AMBIENT, glm::value_ptr(ambient_));
	glLightfv(light_number_, GL_DIFFUSE, glm::value_ptr(diffuse_));
	glLightfv(light_number_, GL_SPECULAR, glm::value_ptr(specular_))*/;

	//if the current light is a directional light, then direction is passed instead
	glLightfv(light_number_, GL_POSITION, glm::value_ptr(position_));

	//light number goes from GL_LIGHT0 to GL_LIGHT7
	if (type_ == SPOTLIGHT)
	{
		glLightfv(light_number_, GL_SPOT_CUTOFF, &cutoff_);
		glLightfv(light_number_, GL_SPOT_DIRECTION, glm::value_ptr(direction_));
	}
	
	for each (BEnode* n in BEnode::children_)
	{
		n->Render(tmp_f);
	}
}

void BElight::RenderSingle(glm::mat4 cumulated_transformation_matrix)
{
	glLoadMatrixf(glm::value_ptr(cumulated_transformation_matrix));

	//Common color property
	glLightfv(light_number_, GL_AMBIENT, glm::value_ptr(ambient_));
	glLightfv(light_number_, GL_DIFFUSE, glm::value_ptr(diffuse_));
	glLightfv(light_number_, GL_SPECULAR, glm::value_ptr(specular_));

	//if the current light is a directional light, then direction is passed instead
	glLightfv(light_number_, GL_POSITION, glm::value_ptr(position_));

	//light number goes from GL_LIGHT0 to GL_LIGHT7
	if (type_ == SPOTLIGHT)
	{
		glLightfv(light_number_, GL_SPOT_CUTOFF, &cutoff_);
		glLightfv(light_number_, GL_SPOT_DIRECTION, glm::value_ptr(direction_));
	}
}

void BElight::CalcTransformation(glm::mat4 cumulated_transformation_matrix)
{
	glm::mat4 tmpF = cumulated_transformation_matrix*transformation_;
	BEengine::lists_->UpdateLight(this, tmpF);

	for each (BEnode* n in BEnode::children_){
		n->CalcTransformation(tmpF);
	}
}

BElight::~BElight()
{
	//this will break further lights
	glDisable(light_number_);
	total_lights--;
}

void BElight::SetAngleInnerCone(float angle_inner_cone)
{
	angle_inner_cone_ = angle_inner_cone;
}

void BElight::SetAngleOuterCone(float angle_outer_cone)
{
	angle_outer_cone_ = angle_outer_cone;
}

void BElight::SetAttenuationConstant(float attenuation_constant)
{
	attenuation_constant_ = attenuation_constant;
}

void BElight::SetAttenuationLinear(float attenuation_linear)
{
	attenuation_linear_ = attenuation_linear;
}

void BElight::SetAttenuationQuadratic(float attenuation_quadratic)
{
	attenuation_quadratic_ = attenuation_quadratic;
}

void BElight::SetAmbient(glm::vec3 ambient)
{
	ambient_ = glm::vec4(ambient, 1.0f);
}

void BElight::SetDiffuse(glm::vec3 diffuse)
{
	diffuse_ = glm::vec4(diffuse, 1.0f);
}

void BElight::SetSpecular(glm::vec3 specular)
{
	specular_ = glm::vec4(specular, 1.0f);
}

void BElight::SetDirection(glm::vec3 direction)
{
	direction_ = direction;
}

void BElight::SetPosition(glm::vec4 position)
{
	position_ = position;
}