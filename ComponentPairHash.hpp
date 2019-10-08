#pragma once
#include <cstddef>
#ifndef ComponentPairHash_hpp
#define ComponentPairHash_hpp


struct ComponentPairHash
{
	/*template <typename T>
	std::size_t operator()(T t) const
	{
		auto valx = t.first->owner->IsQueuedForRemoval();

		if (!valx) {
			std::size_t x = t.first->owner->instanceID->Get();
			std::size_t y = t.second->owner->instanceID->Get();

			return (x >= y) ? (x * x + x + y) : (y * y + y + x);
		}
		else
		{
			std::size_t y = t.second->owner->instanceID->Get();
			return y;
		}
	}*/

	template <typename T>
	std::size_t operator()(T t) const
	{
		

		
			std::size_t x = t.first->owner->instanceID->Get();
			std::size_t y = t.second->owner->instanceID->Get();

			return (x >= y) ? (x * x + x + y) : (y * y + y + x);
		
	}
};

#endif /* ComponentPairHash_hpp */

