// Rob Kohout All Rights Reserved


#include "Controllers/WarriorHeroController.h"
#include "WarriorFunctionLibrary.h"

AWarriorHeroController::AWarriorHeroController()
{
	HeroTeamId = UWarriorFunctionLibrary::GetHeroTeamId();
}

FGenericTeamId AWarriorHeroController::GetGenericTeamId() const
{
	return HeroTeamId;
}
