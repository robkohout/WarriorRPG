// Rob Kohout All Rights Reserved


#include "AbilitySystem/WarriorAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "WarriorDebugHelper.h"

UWarriorAttributeSet::UWarriorAttributeSet()
{
	InitCurrentHealth(1.f);
	InitMaxHealth(1.f);
	InitCurrentRage(1.f);
	InitMaxRage(1.f);
	InitAttackPower(1.f);
	InitDefensePower(1.f);
}

void UWarriorAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());
		SetCurrentHealth(NewCurrentHealth);
	}
	
	if (Data.EvaluatedData.Attribute == GetCurrentRageAttribute())
	{
		const float NewCurrentRage = FMath::Clamp(GetCurrentRage(), 0.f, GetMaxRage());
		SetCurrentRage(NewCurrentRage);
	}
	
	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		const float OldHealth = GetCurrentHealth();
		const float Damage = GetDamageTaken();
		const float NewCurrentHealth = FMath::Clamp(OldHealth - Damage, 0.f, GetMaxHealth());
		SetCurrentHealth(NewCurrentHealth);
		
		const FString DebugString = FString::Printf(
			TEXT("Old Health: %f, Damage: %f, New Health: %f"), 
			OldHealth, Damage, NewCurrentHealth);
		Debug::Print(DebugString, FColor::Green);
		
		// TODO: Notify the UI
		
		// TODO: Handle character death
		if (NewCurrentHealth == 0.f)
		{
			
		}
	}
}
