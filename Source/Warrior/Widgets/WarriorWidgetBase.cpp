// Groundhog & Drake All Rights Reserved.


#include "WarriorWidgetBase.h"

#include "Warrior/Interfaces/PawnUIInterface.h"

void UWarriorWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if(auto* PawnUIInterface = Cast<IPawnUIInterface>(GetOwningPlayerPawn())) {
		if(auto* HeroUIComponent = PawnUIInterface->GetHeroUIComponent()) {
			BP_OnOwningHeroUIComponentInitialized(HeroUIComponent);
		}
	}
}