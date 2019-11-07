#ifndef C_InteractWithObjects_hpp
#define C_InteractWithObjects_hpp

#include "Component.hpp"
#include "C_Direction.hpp"
#include "Raycast.hpp"
#include "BoxCast.hpp"
#include "C_Interactable.hpp"
#include "C_Animation.hpp"

class C_InteractWithObjects : public Component
{
public:
    C_InteractWithObjects(Object* owner);
    
    void Awake() override;
    
    void Update(float deltaTime) override;
    
private:
    std::shared_ptr<C_Direction> direction;
	std::shared_ptr<C_Animation> animation;
    
    float interactionDistance;
	bool m_inDialog = false;
};

#endif /* C_InteractWithObjects_hpp */
