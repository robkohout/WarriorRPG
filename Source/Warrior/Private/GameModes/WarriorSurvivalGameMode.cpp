// Rob Kohout All Rights Reserved


#include "GameModes/WarriorSurvivalGameMode.h"
#include "WarriorDebugHelper.h"
#include "Characters/WarriorEnemyCharacter.h"
#include "Engine/AssetManager.h"

void AWarriorSurvivalGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	checkf(EnemyWaveSpawnerDataTable, TEXT("Forgot to assign a valid data table in survival game mode blueprint"));
	
	SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::WaitSpawnNewWave);
	
	TotalWavesToSpawn = EnemyWaveSpawnerDataTable->GetRowNames().Num();
	
	PreLoadNextWaveEnemies();
}

void AWarriorSurvivalGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (CurrentSurvivalGameModeState == EWarriorSurvivalGameModeState::WaitSpawnNewWave)
	{
		TimePassedSinceStart += DeltaSeconds;
		
		if (TimePassedSinceStart >= SpawnNewWaveWaitTime)
		{
			TimePassedSinceStart = 0.f;
			
			SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::SpawningNewWave);
		}
	}
	
	if (CurrentSurvivalGameModeState == EWarriorSurvivalGameModeState::SpawningNewWave)
	{
		TimePassedSinceStart += DeltaSeconds;
		
		if (TimePassedSinceStart >= SpawnEnemiesDelayTime)
		{
			// TODO: Handle spawn enemies
			
			TimePassedSinceStart = 0.f;
			
			SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::InProgress);
		}
	}
	
	if (CurrentSurvivalGameModeState == EWarriorSurvivalGameModeState::WaveCompleted)
	{
		TimePassedSinceStart += DeltaSeconds;
		
		if (TimePassedSinceStart >= WaveCompletedWaitTime)
		{
			TimePassedSinceStart = 0.f;
			
			CurrentWaveCount++;
			
			if (HasFinishedAllWaves())
			{
				SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::AllWavesDone);
			}
			else
			{
				SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::WaitSpawnNewWave);
				PreLoadNextWaveEnemies();
			}
		}
	}
}

void AWarriorSurvivalGameMode::SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState InState)
{
	CurrentSurvivalGameModeState = InState;
	
	OnSurvivalGameModeStateChanged.Broadcast(CurrentSurvivalGameModeState);
}

bool AWarriorSurvivalGameMode::HasFinishedAllWaves() const
{
	return CurrentWaveCount > TotalWavesToSpawn;
}

void AWarriorSurvivalGameMode::PreLoadNextWaveEnemies()
{
	if (HasFinishedAllWaves()) return;
	
	for (const FWarriorEnemyWaveSpawnerInfo& SpawnerInfo : GetCurrentWaveSpawnerTableRow()->EnemyWaveSpawnerDefinitions)
	{
		if (SpawnerInfo.SoftEnemyClassToSpawn.IsNull()) continue;
		
		UAssetManager::GetStreamableManager().RequestAsyncLoad(
			SpawnerInfo.SoftEnemyClassToSpawn.ToSoftObjectPath(),
			FStreamableDelegate::CreateLambda(
				[SpawnerInfo, this]()
				{
					if (UClass* LoadedEnemyClass = SpawnerInfo.SoftEnemyClassToSpawn.Get())
					{
						PreLoadedEnemyClassMap.Emplace(SpawnerInfo.SoftEnemyClassToSpawn, LoadedEnemyClass);
						Debug::Print(LoadedEnemyClass->GetName() + TEXT(" is loaded"));
					}
				}));
	}
}

FWarriorEnemyWaveSpawnerTableRow* AWarriorSurvivalGameMode::GetCurrentWaveSpawnerTableRow() const
{
	const FName RowName = FName(TEXT("Wave") + FString::FromInt(CurrentWaveCount));
	FWarriorEnemyWaveSpawnerTableRow* FoundRow = EnemyWaveSpawnerDataTable->FindRow<FWarriorEnemyWaveSpawnerTableRow>(RowName, FString());
	
	checkf(FoundRow, TEXT("Could not find a valid row with the name %s in the data table"), *RowName.ToString());
	
	return FoundRow;
}
