#include "C_InteractableTalking.hpp"
#include "S_ScriptProcessor.hpp"
#include "S_Command.hpp"
//#include "SceneDungeon.hpp"


C_InteractableTalking::C_InteractableTalking(Object* owner) : Component(owner), textToSayVector({ "Set Me!" }) {}

void C_InteractableTalking::OnInteraction(Object* other)
{
    /*std::shared_ptr<Object> labelObj = std::make_shared<Object>(owner->context);
    
    auto label = labelObj->AddComponent<C_UIWorldLabel>();
    label->SetDrawLayer(DrawLayer::UI);
    
    label->SetBackgroundSize(sf::Vector2f(120, 120));
    label->SetBackgroundColour(sf::Color::Black);
    
    label->SetFontColour(sf::Color::White);
    label->SetText(textToSay);
    
    labelObj->transform->SetParent(owner->transform);
    labelObj->transform->SetPosition(0.f, -80.f);
    
    owner->context->objects->Add(labelObj);*/
	
	// TODO: what we need to do here now is implement the observer pattern and have a quest system that actually fires from C_Interactable so we dont do this per creature
	// that will be the next thing to do!
	owner->context->scriptEngine->AddCommand(new S_Command_ShowDialog({ textToSayVector }, owner->context->drawTextEngine, owner->context->window));
}
