#pragma once
// DLL
#include "be_dll_interface.h"

#include "be_object.h"
#include "be_includes.h"

class LIB_API BEnode : public BEobject
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
	static BEnode* GetSuperRoot();
	static BEnode* GetSceneRootByName(std::string node_name);

	void AddChild(BEnode *);
	std::vector<BEnode*> GetChildren(){ return children_; }
	void RemoveChild(BEnode *, bool = false);

	void SetParent(BEnode *parent);
	BEnode* GetParent(){ return parent_; }
	void SetTransformation(glm::mat4 transformation){ transformation_ = transformation; }
	glm::mat4 GetTransformation(){ return transformation_; }
	void UpdateTransformationRecursive(glm::mat4 transformation);
	void SetAsSceneRoot();

	//Utility
	virtual BEnode* Find(std::string);
	virtual BEnode* Find(long);
	//empty overridden render method
	//virtual void Render(glm::mat4 world_matrix) override;
	virtual void Render(glm::mat4 world_matrix) override;
	virtual void CalcTransformation(glm::mat4 world_matrix);

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