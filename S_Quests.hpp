#pragma once
#include <string>
#include "S_ScriptProcessor.hpp"


class SceneDungeon;

class S_Quests
{
public:
	//S_Quests(std::string name) {}
	enum NATURE
	{
		TALK = 0,
		ATTACK = 1,
		KILL = 2,
		WALK = 3
	};
	
	

public:
	virtual bool OnInteration(ObjectCollection* objects, Object* target, Object* source, std::string mapname, NATURE nature);
	virtual bool PopulateDynamics(ObjectCollection* objects, std::string mapname, SharedContext* context);

public:
	std::string sName = "Quest Text";
	bool bCompleted = false;
	static S_ScriptProcessor* g_script;
	static HeroClass* g_heroStats;




};

class S_Quests_MainQuest : public S_Quests
{
public:
	
	bool OnInteration(ObjectCollection* objects, Object* target, Object* source, std::string mapname, NATURE nature) override;
	bool PopulateDynamics(ObjectCollection* objects, std::string mapname, SharedContext* context) override;

private:
	int m_nPhase = 0;


};

