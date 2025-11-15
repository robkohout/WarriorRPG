// Rob Kohout All Rights Reserved


#include "Warrior/Public/Characters/WarriorBaseCharacter.h"


AWarriorBaseCharacter::AWarriorBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	GetMesh()->bReceivesDecals = false;
}