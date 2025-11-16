// Rob Kohout All Rights Reserved


#include "AnimInstances/WarriorCharacterAnimInstance.h"
#include "Characters/WarriorBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UWarriorCharacterAnimInstance::NativeInitializeAnimation()
{
	OwningCharacter = Cast<AWarriorBaseCharacter>(TryGetPawnOwner());
	
	if (OwningCharacter)
	{
		OwningCharacterMovement = OwningCharacter->GetCharacterMovement();
	}
}

void UWarriorCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (!OwningCharacter || !OwningCharacterMovement) return;
	
	GroundSpeed = OwningCharacter->GetVelocity().Size2D();
	
	bHasAcceleration = OwningCharacterMovement->GetCurrentAcceleration().SizeSquared2D() > 0.f;
}
