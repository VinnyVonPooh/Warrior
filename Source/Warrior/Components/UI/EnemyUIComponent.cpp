// Groundhog & Drake All Rights Reserved.


#include "EnemyUIComponent.h"

#include "Warrior/Widgets/WarriorWidgetBase.h"

void UEnemyUIComponent::RegisterEnemyDrawWidget(UWarriorWidgetBase* InWidgetToRegister)
{
	EnemyDrawnWidjets.Add(InWidgetToRegister);
}

void UEnemyUIComponent::RemoveEnemyDrawWidgetIfAny()
{
	if (EnemyDrawnWidjets.IsEmpty()) {
		return;
	}

	for (auto* DrawnWidget : EnemyDrawnWidjets) {
		if (DrawnWidget) {
			DrawnWidget->RemoveFromParent();
		}
	}
}