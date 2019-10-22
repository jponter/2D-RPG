#pragma once
#include "S_Command.hpp"
#include <list>

class S_ScriptProcessor
{
public:
	S_ScriptProcessor();

	void AddCommand(S_Command* cmd);
	void ProcessCommand(float fElapsedTime);
	void CompleteCommand();

	bool bUserControlEnabled;

private:
	std::list<S_Command*> m_listCommands;
};

