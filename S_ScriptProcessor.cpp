#include "S_ScriptProcessor.hpp"

S_ScriptProcessor::S_ScriptProcessor()
{
	bUserControlEnabled = true;
}

void S_ScriptProcessor::AddCommand(S_Command* cmd)
{
	m_listCommands.push_back(cmd);
}

void S_ScriptProcessor::ProcessCommand(float fElapsedTime)
{
	//if commands are available halt control to the player
	bUserControlEnabled = m_listCommands.empty();

	if (!m_listCommands.empty())
	{

		//a command is available
		if (!m_listCommands.front()->bCompleted)
		{
			// command has not been started
			if (!m_listCommands.front()->bStarted)
			{
				m_listCommands.front()->Start();
				m_listCommands.front()->bStarted = true;
			}
			else //command has been started - let's update
				m_listCommands.front()->Update(fElapsedTime);
		}
		else //command has been completed
		{
			delete m_listCommands.front();
			m_listCommands.pop_front();
			bUserControlEnabled = true;
		}
	}
}

void S_ScriptProcessor::CompleteCommand()
{
	if (!m_listCommands.empty())
	{
		m_listCommands.front()->bCompleted = true;
	}
}
