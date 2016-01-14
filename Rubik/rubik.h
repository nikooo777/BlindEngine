#pragma once
#include "be_engine.h"

class Rubik
{
public:
	Rubik(BEnode*);
	~Rubik();

	void TranslateSingleCubeX(unsigned short element_x, unsigned short element_y, unsigned short element_z, float value_x);
	void TranslateSingleCubeY(unsigned short element_x, unsigned short element_y, unsigned short element_z, float value_y);
	void TranslateSingleCubeZ(unsigned short element_x, unsigned short element_y, unsigned short element_z, float value_z);
	void TranslateSingleCube(unsigned short element_x, unsigned short element_y, unsigned short element_z, float value_x, float value_y, float value_z);

private:
	/*
	    .+------+	UP: GREEN
	  .' |    .'|	RIGHT: WHITE
	 +---+--+'  |   FRONT: RED
	 |   |  |   |  
	 |  ,+--+---+  
	 |.'    | .'   
	 +------+'

	 RED bottom-left: [0][0][0] and then as axis

	 ---> [+][0][0]  (x)
	 
	 ^
	 |	[0][+][0]   (y)
	 |

	    ^
	   /  [0][0][+]   (z)
	  / 
	 /

	 */

	BEnode* cube_faces_[3][3][3];
};

