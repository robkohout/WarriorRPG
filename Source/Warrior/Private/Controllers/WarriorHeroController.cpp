// Rob Kohout All Rights Reserved


#include "Controllers/WarriorHeroController.h"
#include "WarriorFunctionLibrary.h"

AWarriorHeroController::AWarriorHeroController()
{
	HeroTeamId = UWarriorFunctionLibrary::NativeGetHeroTeamId();
}

FGenericTeamId AWarriorHeroController::GetGenericTeamId() const
{
	return HeroTeamId;
}
