#pragma once
#include "be_object.h"
#include "be_includes.h"

class BEnode : public BEobject
{
public:
	//types
	typedef enum Type
	{
		CAMERA,
		MESH,
		LIGHT,
		ROOT
	}Type;

	//constructor and destructor
	BEnode(std::string name, Type type);
	virtual ~BEnode();

	//functions
	static BEnode* GetSuperRoot(){ return super_root_node; }

	void AddChild(BEnode *);
	std::vector<BEnode*> GetChildren(){ return children_; }
	void RemoveChild(BEnode *, bool = false);

	void SetParent(BEnode *parent){ parent_ = parent; }
	void SetTransformation(glm::mat4 transformation){ transformation_ = transformation; }
	void SetAsRoot(){ parent_ = this;/*GetSuperRoot();*/ }

	//Utility
	BEnode* find(std::string);
	BEnode* find(long);
	//empty overridden render method
	virtual void Render(glm::mat4) override;

protected:
	// Fields common in all children
	BEnode* parent_;
	glm::mat4 transformation_;
	std::vector<BEnode*> children_;

private:
	//members
	Type type_;
	static BEnode* super_root_node;
};