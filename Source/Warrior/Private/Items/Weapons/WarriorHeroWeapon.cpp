// Rob Kohout All Rights Reserved


#include "Items/Weapons/WarriorHeroWeapon.h"


void AWarriorHeroWeapon::AssignGrantedAbilitySecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles)
{
	GrantedAbilitySpecHandles = InSpecHandles;
}

TArray<FGameplayAbilitySpecHandle> AWarriorHeroWeapon::GetGrantedAbilitySpecHandles() const
{
	return GrantedAbilitySpecHandles;
}
