#pragma once
#include "be_object.h"
#include "be_includes.h"

class BEnode : public BEobject
{
public:
	//constructor and destructor
	virtual ~BEnode();

	//functions
	static BEnode* GetSuperRoot(){ return super_root_node; }
	
	void AddChild(BEnode *);
	std::vector<BEnode*> GetChildren(){ return children_; }
	void RemoveChild(BEnode * node, bool = false);

	void SetName(std::string name){ name_ = name; }
	void SetParent(BEnode *parent){ parent_ = parent; }
	void SetTransformation(glm::mat4 transformation){ transformation_ = transformation; }
	void SetAsRoot(){ parent_ = GetSuperRoot(); }

	//Utility
	BEnode* find(std::string name);
	BEnode* find(long id);
	void PrintName(){ std::cout << name_ << std::endl; }

	virtual void Render() = 0;

protected:
	// Fields common in all children
	BEnode();
	BEnode* parent_;
	std::string name_;
	glm::mat4 transformation_;
	std::vector<BEnode*> children_;

	//members
private:
	//members
	static BEnode* super_root_node;
};

