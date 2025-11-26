// Rob Kohout All Rights Reserved


#include "Controllers/WarriorAIController.h"
#include "WarriorFunctionLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


AWarriorAIController::AWarriorAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
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
	
	SetGenericTeamId(UWarriorFunctionLibrary::NativeGetEnemyTeamId());
}

ETeamAttitude::Type AWarriorAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* PawnToCheck = Cast<const APawn>(&Other);
	
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(PawnToCheck->GetController());
	
	if (OtherTeamAgent && OtherTeamAgent->GetGenericTeamId() == UWarriorFunctionLibrary::NativeGetHeroTeamId())
	{
		return ETeamAttitude::Hostile;
	}
	
	return ETeamAttitude::Friendly;
}

void AWarriorAIController::BeginPlay()
{
	Super::BeginPlay();
	
	if (UCrowdFollowingComponent* CrowdComponent = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		CrowdComponent->SetCrowdSimulationState(bEnableDetourCrowdAvoidance ? ECrowdSimulationState::Enabled : ECrowdSimulationState::Disabled);
		
		switch (DetourCrowdAvoidanceQuality)
		{
		case 1:	CrowdComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Low);		break;
		case 2: CrowdComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Medium);	break;
		case 3:	CrowdComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good);		break;
		case 4: CrowdComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);		break;
		default: break;
		}
		
		CrowdComponent->SetAvoidanceGroup(UWarriorFunctionLibrary::NativeGetEnemyTeamId());
		CrowdComponent->SetGroupsToAvoid(UWarriorFunctionLibrary::NativeGetEnemyTeamId());
		CrowdComponent->SetCrowdCollisionQueryRange(CollisionQueryRange);
	}
}

void AWarriorAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
	{
		if (!BlackboardComponent->GetValueAsObject(FName("TargetActor")))
		{
			if (Stimulus.WasSuccessfullySensed() && Actor)
			{
				BlackboardComponent->SetValueAsObject(FName("TargetActor"), Actor);
			}
		}
	}
}
