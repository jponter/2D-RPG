#ifndef C_Interactable_hpp
#define C_Interactable_hpp

#include "Object.hpp"

class C_Interactable
{
public:
    virtual bool OnInteraction(Object* other) = 0;
};

#endif /* C_Interactable_hpp */
