#pragma once
#include "be_engine.h"
#include <glm/gtx/constants.hpp>
class Rubik
{
public:
	Rubik(BEnode*);
	~Rubik();
	typedef enum Face
	{
		U_FACE,
		R_FACE,
		D_FACE,
		L_FACE,
		F_FACE,
		B_FACE,
	}Face;

	void TranslateSingleCubeX(unsigned short element_x, unsigned short element_y, unsigned short element_z, float value_x);
	void TranslateSingleCubeY(unsigned short element_x, unsigned short element_y, unsigned short element_z, float value_y);
	void TranslateSingleCubeZ(unsigned short element_x, unsigned short element_y, unsigned short element_z, float value_z);
	void TranslateSingleCube(unsigned short element_x, unsigned short element_y, unsigned short element_z, float value_x, float value_y, float value_z);
	void RotateFace(Face face, bool inverse);

	void PushBackFace(BEnode** faces_to_swap, BEnode* rotation_helper);

	void RelinkCubes(BEnode** faces_to_swap, BEnode* rotation_helper);

private:
	/*
	*	    .+------+	UP: GREEN
	*	  .' |    .'|	RIGHT: WHITE
	*	 +---+--+'  |   FRONT: RED
	*	 |   |  |   |
	*	 |  ,+--+---+
	*	 |.'    | .'
	*	 +------+'
	*
	*	 RED bottom-left: [0][0][0] and then as axis
	*
	*	 -----> [+][0][0]  (x)
	*
	*	 ^
	*	 |	[0][+][0]   (y)
	*	 |
	*	 |
	*	 
	*	    ^
	*	   /  [0][0][+]   (z)
	*	  /
	*	 /
	*
	* green face:	[*][3][*]	center cube: [2][3][2]
	* white face:	[3][*][*]	center cube: [3][2][2]
	* red face:		[*][*][0]	center cube: [2][2][0]
	* blue face:	[*][0][*]	center cube: [2][0][2]
	* yellow face:	[0][*][*]	center cube: [0][2][2]
	* orange face:	[*][*][3]	center cube: [2][2][3]
	*/

	BEnode* cube_faces_[3][3][3];
	BEnode* cube_root_;
	BEnode* get_green()	{ return cube_faces_[2][3][2]; }
	BEnode* get_white()	{ return cube_faces_[3][2][2]; }
	BEnode* get_red()	{ return cube_faces_[2][2][0]; }
	BEnode* get_blue()	{ return cube_faces_[2][0][2]; }
	BEnode* get_yellow(){ return cube_faces_[0][2][2]; }
	BEnode* get_orange(){ return cube_faces_[2][2][3]; }

};

