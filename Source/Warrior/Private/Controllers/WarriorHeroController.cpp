// Rob Kohout All Rights Reserved


#include "Controllers/WarriorHeroController.h"
#include "WarriorTypes/WarriorEnumTypes.h"

AWarriorHeroController::AWarriorHeroController()
{
	HeroTeamId = FGenericTeamId(static_cast<uint8>(EWarriorTeamType::Hero));
}

FGenericTeamId AWarriorHeroController::GetGenericTeamId() const
{
	return HeroTeamId;
}
