#pragma once
#include <imgui.h>
#include <imgui-SFML.h>


class Logger
{
public:
	virtual void    AddLog(const char* fmt, ...) IM_FMTARGS(2) = 0;
	virtual void    Draw(const char* title, bool* p_open = NULL) = 0;
	//virtual ~Logger() {};



};

