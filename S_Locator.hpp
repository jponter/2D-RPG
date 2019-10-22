#pragma once
#include "Logger.hpp"


class S_Locator
{
public:
	

	static Logger* getLogger() { return service_; };

	static void provide(Logger* service)
	{
		service_ = service;
	}

	static Logger* service_;

private:
	

};

