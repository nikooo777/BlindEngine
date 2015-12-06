#pragma once
/*
	SINGLETON class
*/
class engine
{
public:
	static engine* instance();
    void init();
private:

	static engine* __instance;

	engine();
	~engine();
};

