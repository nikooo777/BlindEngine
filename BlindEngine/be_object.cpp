#include "be_object.h"

void BEobject::GenerateId()
{
	static long int id = 0;
	id_ = id++;
}

BEobject::BEobject(std::string name)
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