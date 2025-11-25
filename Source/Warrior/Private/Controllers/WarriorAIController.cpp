// Rob Kohout All Rights Reserved


#include "Controllers/WarriorAIController.h"
#include "WarriorDebugHelper.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


AWarriorAIController::AWarriorAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		Debug::Print(TEXT("CrowdFollowingComponent valid"), FColor::Green);
	}
	
	EnemySenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("EnemySenseConfig_Sight"));
	EnemySenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	EnemySenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	EnemySenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;
	EnemySenseConfig_Sight->SightRadius = 5000.f;
	EnemySenseConfig_Sight->LoseSightRadius = 0.f;
	EnemySenseConfig_Sight->PeripheralVisionAngleDegrees = 360.f;
	
	EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("EnemyPerceptionComponent"));
	EnemyPerceptionComponent->ConfigureSense(*EnemySenseConfig_Sight);
	EnemyPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::OnEnemyPerceptionUpdated);
	
	SetGenericTeamId(FGenericTeamId(1));
}

ETeamAttitude::Type AWarriorAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* PawnToCheck = Cast<const APawn>(&Other);
	
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(PawnToCheck->GetController());
	
	if (OtherTeamAgent && OtherTeamAgent->GetGenericTeamId() != GetGenericTeamId())
	{
		return ETeamAttitude::Hostile;
	}
	
	return ETeamAttitude::Friendly;
}

void AWarriorAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed() && Actor)
	{
		Debug::Print(Actor->GetActorNameOrLabel() + TEXT(" was sensed"), FColor::Green);
	}
}
