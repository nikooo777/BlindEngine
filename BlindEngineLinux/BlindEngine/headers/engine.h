#pragma once
/*
	SINGLETON class
*/
class engine
{
public:
	static engine* instance();

private:

	static engine* __instance;

	engine();
	~engine();
};

