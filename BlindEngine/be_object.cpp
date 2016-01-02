#include "be_object.h"

long int BEobject::GenerateId()
{
	static long int id = 0;
	return id++;
}

