#ifndef C_InteractableTalking_hpp
#define C_InteractableTalking_hpp

#include "Component.hpp"
#include "C_Interactable.hpp"
#include "Debug.hpp"
#include "C_UIWorldLabel.hpp"
#include "ObjectCollection.hpp"
#include "DrawText.h"

class C_InteractableTalking : public Component, public C_Interactable
{
public:
    C_InteractableTalking(Object* owner);
    
    void OnInteraction(Object* other) override;

	/*void SetTextEngine(const DrawText& engine)
	{
		drawTextEngine = engine;
	}*/

	void SetText(std::vector<std::string> textvec)
	{
		textToSayVector = textvec;
	}
    
private:
    std::vector<std::string> textToSayVector;
	//DrawText& drawTextEngine;
};

#endif /* C_InteractableTalking_hpp */
