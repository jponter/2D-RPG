#include "S_Quests.hpp"
#include "Debug.hpp"
#include "SharedContext.hpp"
#include "Scene.hpp"

#define X(n) g_script->AddCommand(new S_Command_ ## n) // shorthand for adding scripts to the script engine

S_ScriptProcessor* S_Quests::g_script = nullptr;
HeroClass* S_Quests::g_heroStats = nullptr;





bool S_Quests::OnInteration(ObjectCollection* objects, Object* target, Object* source, std::string mapname, NATURE nature)
{
	Debug::Log("S_Quests::OnInteration");
	return true;
}

bool S_Quests::PopulateDynamics(ObjectCollection* objects, std::string mapname, SharedContext* context)
{
	return false;
}





bool S_Quests_MainQuest::PopulateDynamics(ObjectCollection* objects, std::string mapname, SharedContext* context)
{
	
	if (mapname == "House Exterior")
	{
		if (m_nPhase == 0)
		{
			context->currentScene->AddNpcToScene("Luna", 400, 400, "Orc", false);
			Debug::Log("Added a Luna to Exterior from PopulateDynamics");
		}
		
	}

	if (mapname == "Creepy Dungeon")
	{
		if (m_nPhase > 0)
		{
			//Luna is in the dungeon for all the other phases
			context->currentScene->AddNpcToScene("Luna", 400, 400, "Orc", false);
			Debug::Log("Added a Luna to Dungeon from PopulateDynamics");
		}
	}

	return true;
}



bool S_Quests_MainQuest::OnInteration(ObjectCollection* objects, Object* target, Object* source, std::string mapname, NATURE nature)
{
	Debug::Log("S_Quests_MainQuest::OnInteration Target = " + std::to_string(target->instanceID->Get()) + " Target Name: " + target->name);

	if (target->name == "Skelly") {

		if (g_heroStats->atMaxHealth())
		{
			
			//g_script->AddCommand(new S_Command_ShowDialog({ "[Skelly]","Hello " + source->name,"You are at Max Health!" }, target->context->drawTextEngine, target->context->window));
			X(ShowDialog({ "[Skelly]","Hello " + source->name,"You are at Max Health!" }, target->context->drawTextEngine, target->context->window));
		}
		else
		{
			g_script->AddCommand(new S_Command_ShowDialog({ "[Skelly]", "Hello" }, target->context->drawTextEngine, target->context->window));
		}
		return true;
	}

	if (target->name == "Jim") {
		g_script->AddCommand(new S_Command_ShowDialog({ "[Jim]", "Hello " +  source->name }, target->context->drawTextEngine, target->context->window));
		return true;
	}

	if (target->name == "Orcy")
	{
		if (source->context->playerInventory->HasItem("HEALTH"))
		{
			X(ShowDialog({ "[Orcy]" , "Hey " + source->name , "you have a health potion! Cool!" }, target->context->drawTextEngine, target->context->window));
			return true;
		}
		else return false;
	}


	if (target->name == "Luna") {
		if (m_nPhase == 0)
		{
			g_script->AddCommand(new S_Command_ShowDialog({ "[Luna]", "Hi " + source->name, "I'm going to the Dungeon!" }, target->context->drawTextEngine, target->context->window));
			m_nPhase++;
			return true;
		}
		else if (m_nPhase > 0)
		{
			if (mapname == "Creepy Dungeon") {
				g_script->AddCommand(new S_Command_ShowDialog({ "[Luna]", "Hi " + source->name, "Thanks for following me to the Dungeon!" }, target->context->drawTextEngine, target->context->window));
				return true;
			}
			else
			{
				X(ShowDialog({ "[Luna]","I'll meet you there in a minute!" }, target->context->drawTextEngine, target->context->window));
				return true;
			}
		}
	}


	return false;
}


