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
		//OLD_gl //glEnable(light_number_);
	}
	else
		throw new std::runtime_error("There are too many lights in the scene!");

	ambient_ = ambient;
	diffuse_ = diffuse;
	specular_ = specular;
	type_ = type;
	cutoff_ = cutoff;
	direction_ = direction;

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
	return new BElight(OMNIDIRECTIONAL, name, glm::vec4(ambient, 1.0f), glm::vec4(diffuse, 1.0f), glm::vec4(specular, 1.0f), position, glm::vec3(), 180.f);
}

BElight* BElight::CreateDirectionalLight(const std::string name, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction)
{
	return new BElight(DIRECTIONAL, name, glm::vec4(ambient, 1.0f), glm::vec4(diffuse, 1.0f), glm::vec4(specular, 1.0f), glm::vec3(), direction, 0.f);
}

BElight* BElight::CreateSpotLight(const std::string name, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, glm::vec3 direction, float cutoff)
{
	return new BElight(SPOTLIGHT, name, glm::vec4(ambient, 1.0f), glm::vec4(diffuse, 1.0f), glm::vec4(specular, 1.0f), position, direction, cutoff);
}

void BElight::Render(glm::mat4 world_matrix)
{

	if (!is_active_)
	{
		//OLD_gl //glDisable(light_number_);
		return;
	}

	//OLD_gl //glEnable(light_number_);
	//OLD_gl //glLoadMatrixf(glm::value_ptr(world_matrix));
	//Common color property
	//OLD_gl //glLightfv(light_number_, GL_AMBIENT, glm::value_ptr(ambient_));
	//OLD_gl //glLightfv(light_number_, GL_DIFFUSE, glm::value_ptr(diffuse_));
	//OLD_gl //glLightfv(light_number_, GL_SPECULAR, glm::value_ptr(specular_));

	//if the current light is a directional light, then direction is passed instead
	//OLD_gl //glLightfv(light_number_, GL_POSITION, glm::value_ptr(position_));
	BEshader* shader = BEengine::GetInstance()->get_shader();

	if (type_ == SPOTLIGHT)
	{
		float new_cutoff = cutoff_ / 2.f;
		//std::cout <<"cutoff " << new_cutoff << std::endl;
		//OLD_gl //glLightfv(light_number_, GL_SPOT_CUTOFF, &new_cutoff);
		//OLD_gl //glLightfv(light_number_, GL_SPOT_DIRECTION, glm::value_ptr(direction_));
		//OLD_gl //glLightfv(light_number_, GL_QUADRATIC_ATTENUATION, &attenuation_quadratic_);
		//OLD_gl //glLightfv(light_number_, GL_CONSTANT_ATTENUATION, &attenuation_constant_);
		//OLD_gl //glLightfv(light_number_, GL_LINEAR_ATTENUATION, &attenuation_linear_);
	}
	if (type_ == OMNIDIRECTIONAL)
	{
		//OLD_gl //glLightfv(light_number_, GL_SPOT_CUTOFF, &cutoff_);
		//printf("%f - %f - %f\n", mat[3][0], mat[3][1], mat[3][2]);
		shader->setVec3(shader->lightPositionLoc, glm::vec3(world_matrix*position_));

		//m_shader->setVec3(m_shader->lightPositionLoc, TE_VEC3(0.0f,5.0f,-5.0f));
		shader->setVec3(shader->lightAmbientLoc, glm::vec3(ambient_));
		shader->setVec3(shader->lightDiffuseLoc, glm::vec3(diffuse_));
		shader->setVec3(shader->lightSpecularLoc, glm::vec3(specular_));
	}
}

void BElight::CalcTransformation(glm::mat4 world_matrix)
{
	glm::mat4 tmpF = world_matrix*transformation_;
	BEengine::lists_->Pass(this, tmpF);

	for (BEnode* n : BEnode::children_){
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


BEnode* BElight::Find(std::string name)
{
	//the current node is the one sought
	if (this->get_name().compare(name) == 0)
	{
		return this;
	}

	//seek the node in the children
	BEnode *found_node = nullptr;
	for (BEnode* n : children_)
	{
		if ((found_node = n->Find(name)) != nullptr)
			return found_node;
	}
	//there are no more children to look into
	return nullptr;
}

BEnode* BElight::Find(long id)
{
	//the current node is the one sought
	if (this->get_id() == id)
	{
		return this;
	}

	//seek the node in the children
	BEnode *found_node = nullptr;
	for (BEnode* n : children_)
	{
		if ((found_node = n->Find(id)) != nullptr)
			return found_node;
	}
	//there are no more children to look into
	return nullptr;
}
