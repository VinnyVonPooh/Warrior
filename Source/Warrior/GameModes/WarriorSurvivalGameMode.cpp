// Groundhog & Drake All Rights Reserved.


#include "WarriorSurvivalGameMode.h"

void AWarriorSurvivalGameMode::BeginPlay()
{
	Super::BeginPlay();

	check(EnemyWaveSpawnerDataTable);

	SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::WaitSpawnNewWave);

	TotalWavesToSpawn = EnemyWaveSpawnerDataTable->GetRowNames().Num();
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
			//TODO

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
	return CurrentWaveCount >= TotalWavesToSpawn;
}