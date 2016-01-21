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
		M_FACE,
		MF_FACE,
		ML_FACE
	}Face;

	void RotateFace(Face face, bool inverse);
	//void PushBackFace(BEnode** faces_to_swap, BEnode* rotation_helper);
	//void RelinkCubes(BEnode** faces_to_swap, BEnode* rotation_helper);
	void BuildSceneGraph(BEnode* rotation_helper, BEnode** faces_to_swap);

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
};

