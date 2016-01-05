#pragma once
#include "be_object.h"
#include "be_includes.h"

class BEnode : public BEobject
{
public:
	//constructor and destructor
	BEnode(const std::string);
	BEnode();
	virtual ~BEnode();
	//functions
	void AddChild(BEnode *);
	virtual void Render() override;
	void ChangeParent(BEnode *);
	void RemoveChild(BEnode * node, bool = false);
	BEnode* find(std::string name);
	BEnode* find(long id);
	void SetPosition(glm::mat4);
	void SetAsRoot();

	//members
private:
	//constructor
	//functions
	static BEnode* GetRoot();
	std::vector<BEnode*> GetChildren();

	//members
	static BEnode* super_root_node;
	glm::mat4 position_;
	BEnode* parent_;
	std::vector<BEnode*> children_;
};

