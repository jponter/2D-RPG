#pragma once
#ifndef Utilities_h
#define Utilities_h

#include <string>
#include <math.h>
#include <corecrt_math_defines.h>
#include <SFML/System/Vector2.hpp>

class Utilities
{
public:
	// Used answers from: 
	// https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c
		//TODO: not robust. Only correctly handles whole positive numbers.
	static inline bool IsInteger(const std::string& s)
	{
		if (s.empty()
			|| ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+')))
		{
			return false;
		}

		char* p;
		strtol(s.c_str(), &p, 10);
		return (*p == 0);
	}

	//p1 = Point 1 , p2 = Point 2, p3 = Origin
	//returns angle between P1 and P2 with respect to the origin point
	static double AngleBetweenThreePoints(const sf::Vector2f& p1, const sf::Vector2f& p2,const  sf::Vector2f& p3)
	{
		
		double atanResult = atan2(p3.y - p1.y, p3.x - p1.x) - atan2(p2.y - p1.y, p2.x - p1.x);

		if (atanResult < 0) atanResult += M_PI * 2;

		atanResult = atanResult * (180 / M_PI);

		return atanResult;
	}

	static inline float DistanceBetweenTwoPoints(const sf::Vector2f& p1, const sf::Vector2f& p2)
	{
		return sqrt(((p2.x - p1.x) * (p2.x - p1.x)) + ((p2.y - p1.y) * (p2.y - p1.y)));

	}

};

#endif /* Utilities_h */

