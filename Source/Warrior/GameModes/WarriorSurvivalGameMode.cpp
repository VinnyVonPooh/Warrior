// Groundhog & Drake All Rights Reserved.

#include "WarriorSurvivalGameMode.h"

#include "NavigationSystem.h"
#include "Engine/AssetManager.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Warrior/Characters/WarriorEnemyCharacter.h"

void AWarriorSurvivalGameMode::BeginPlay()
{
	Super::BeginPlay();

	check(EnemyWaveSpawnerDataTable);

	SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::WaitSpawnNewWave);

	TotalWavesToSpawn = EnemyWaveSpawnerDataTable->GetRowNames().Num();

	PreLoadNextWaveEnemies();
}

void AWarriorSurvivalGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CurrentSurvivalGameModeState == EWarriorSurvivalGameModeState::WaitSpawnNewWave) {
		TimePassedSinceStart += DeltaSeconds;

		if (TimePassedSinceStart >= SpawnNewWaveWaitTime) {
			TimePassedSinceStart = 0.f;
			SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::SpawningNewWave);
		}
	}
	if (CurrentSurvivalGameModeState == EWarriorSurvivalGameModeState::SpawningNewWave) {
		TimePassedSinceStart += DeltaSeconds;

		if (TimePassedSinceStart >= SpawnEnemiesDelayTime) {
			CurrentSpawnedEnemiesCounter += TrySpawnWaveEnemies();

			TimePassedSinceStart = 0.f;
			SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::InProgress);
		}
	}

	if (CurrentSurvivalGameModeState == EWarriorSurvivalGameModeState::WaveCompleted) {
		TimePassedSinceStart += DeltaSeconds;

		if (TimePassedSinceStart >= WaveCompletedWaitTime) {
			TimePassedSinceStart = 0.f;

			CurrentWaveCount++;
			if (HasFinishedAllWaves()) {
				SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::AllWavesDone);
			} else {
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
	if (HasFinishedAllWaves()) {
		return;
	}

	for (const auto& SpawnerInfo : GetCurrentWaveSpawnerTableRow()->EnemyWaveSpawnerDefinitions) {
		if (SpawnerInfo.SoftEnemyClassToSpawn.IsNull()) {
			continue;
		}

		UAssetManager::GetStreamableManager().RequestAsyncLoad(SpawnerInfo.SoftEnemyClassToSpawn.ToSoftObjectPath(), //
															   FStreamableDelegate::CreateLambda([SpawnerInfo, this]() {
																   if (UClass* LoadedEnemyClass = SpawnerInfo.SoftEnemyClassToSpawn.Get()) {
																	   PreloadedEnemyClassMap.Emplace(SpawnerInfo.SoftEnemyClassToSpawn,
																									  LoadedEnemyClass);
																   }
															   }));
	}
}

FWarriorEnemyWaveSpawnerTableRow* AWarriorSurvivalGameMode::GetCurrentWaveSpawnerTableRow() const
{
	const FName RawName = FName(TEXT("Wave") + FString::FromInt(CurrentWaveCount));
	auto* FoundRow = EnemyWaveSpawnerDataTable->FindRow<FWarriorEnemyWaveSpawnerTableRow>(RawName, FString());
	check(FoundRow);
	return FoundRow;
}

int32 AWarriorSurvivalGameMode::TrySpawnWaveEnemies()
{
	if (TargetPointsArray.IsEmpty()) {
		UGameplayStatics::GetAllActorsOfClass(this, ATargetPoint::StaticClass(), TargetPointsArray);
	}
	check(!TargetPointsArray.IsEmpty());

	uint32 EnemiesSpawnedThisTime = 0;
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (const auto& SpawnerInfo : GetCurrentWaveSpawnerTableRow()->EnemyWaveSpawnerDefinitions) {
		if (SpawnerInfo.SoftEnemyClassToSpawn.IsNull()) {
			continue;
		}
		const int32 NumToSpawn = FMath::RandRange(SpawnerInfo.MinPerSpawnCount, SpawnerInfo.MaxPerSpawnCount);

		UClass* LoadedEnemyClass = PreloadedEnemyClassMap.FindChecked(SpawnerInfo.SoftEnemyClassToSpawn);

		for (int32 i = 0; i < NumToSpawn; ++i) {
			const int32 RandomTargetPointIndex = FMath::RandRange(0, TargetPointsArray.Num() - 1);
			const FVector SpawnOrigin = TargetPointsArray[RandomTargetPointIndex]->GetActorLocation();
			const FRotator SpawnRotation = TargetPointsArray[RandomTargetPointIndex]->GetActorForwardVector().ToOrientationRotator();

			FVector RandomLocation;
			UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(this, SpawnOrigin, RandomLocation, 400.f);

			RandomLocation += FVector(0.f, 0.f, 150.f);

			auto* SpawnedEnemy =
				GetWorld()->SpawnActor<AWarriorEnemyCharacter>(LoadedEnemyClass, RandomLocation, SpawnRotation, SpawnParam);

			if (SpawnedEnemy) {
				++EnemiesSpawnedThisTime;
				++TotalSpawnedEnemiesThisWaveCounter;
			}

			if (!ShouldKeepSpawnEnemies()) {
				return EnemiesSpawnedThisTime;
			}
		}
	}

	return EnemiesSpawnedThisTime;
}

bool AWarriorSurvivalGameMode::ShouldKeepSpawnEnemies() const
{
	return TotalSpawnedEnemiesThisWaveCounter < GetCurrentWaveSpawnerTableRow()->TotalEnemyToSpawnThisWave;
}