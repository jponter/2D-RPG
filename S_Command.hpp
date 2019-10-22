#pragma once
#include "Object.hpp"
#include "C_Velocity.hpp"
#include <algorithm>
#include "DrawText.h"


class S_Command
{

public:
	S_Command() {}
	//virtual ~S_Command();

	bool bCompleted = false;
	bool bStarted = false;

	virtual void Start() {};
	virtual void Update(float fElapsedTime) {};
};

class S_Command_MoveTo : public S_Command
{
public: 
	S_Command_MoveTo(std::shared_ptr<Object>& object, float x, float y, float duration = 0.0f);
	void Start() override;
	void Update(float fElapsedTime) override;

private:
	std::shared_ptr<Object>& m_pObject;
	float m_fStartPosX;
	float m_fStartPosY;
	float m_fTargetPosX;
	float m_fTargetPosY;
	float m_fDuration;
	float m_fTimeSoFar;



};

class  S_Command_ShowDialog : public S_Command
{
public:
	S_Command_ShowDialog(std::vector<std::string> line, DrawText& DrawTextEngine, Window& window);
	void Start() override;

private:
	std::vector<std::string> vecLines;
	DrawText& DrawTextEngine;
	Window& window;

};
	
	

