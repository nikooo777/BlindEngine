#include "be_object.h"

BEobject::~BEobject()
{

}

BEobject::BEobject(const std::string name)
{
	GenerateId();
	name_ = name;
}

BEobject::BEobject()
{
	GenerateId();
	std::stringstream sstm;
	sstm << "object_" << id_;
	name_ = sstm.str();
}

void BEobject::GenerateId()
{
	static long int id = 0;
	id_ = id++;
}
