// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "Tower.h"
#include "BattleBlasterGameMode.h"
#include "BattleBlasterGameInstance.h"


void ABattleBlasterGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Towers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), Towers);
	TowerCount = Towers.Num();
	UE_LOG(LogTemp, Display, TEXT("Number of towers: %d"), TowerCount);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	
	if (PlayerPawn) {
		Tank = Cast<ATank>(PlayerPawn);
		if (!Tank) {
			UE_LOG(LogTemp, Display, TEXT("GameMode failed to find the tank actor!"));
		}
	}

	int32 LoopIndex = 0;
	while (LoopIndex < TowerCount) {

		AActor* TowerActor = Towers[LoopIndex];
		if (TowerActor) {
			ATower * Tower = Cast<ATower>(TowerActor);
			if (Tower && Tank) {
				Tower->Tank = Tank;
				UE_LOG(LogTemp, Display, TEXT("%s setting the tank variable"), *Tower->GetActorNameOrLabel());
			}
		}

		LoopIndex++;
	}

	CountdownSeconds = CountdownDelay;
	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ABattleBlasterGameMode::OnCountdownTimerTimeout, 1.0f, true);
	
}

void ABattleBlasterGameMode::OnCountdownTimerTimeout()
{
	CountdownSeconds--;
	if (CountdownSeconds > 0) {
		UE_LOG(LogTemp, Display, TEXT("Countdown: %d"), CountdownSeconds);
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("GO!"));
		Tank->SetPlayerEnabled(true);
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
	}
	
	

}

void ABattleBlasterGameMode::ActorDied(AActor* DeadActor)
{
	bool IsGameOver = false;

	if (DeadActor == Tank) {
		Tank->HandleDestruction();
		IsGameOver = true;
	}
	else {
		ATower* DeadTower = Cast<ATower>(DeadActor);
		if (DeadTower) {
			DeadTower->HandleDestruction();
			
			TowerCount--;
			if (TowerCount == 0) {
				IsGameOver = true;
				IsVictory = true;
			}
		}
	}

	if (IsGameOver) {
		FString GameOverString = IsVictory ? "Victory!" : "Defeat!";
		UE_LOG(LogTemp, Display, TEXT("Game over: %s"), *GameOverString);

		FTimerHandle GameOverTimerHandle;
		GetWorldTimerManager().SetTimer(GameOverTimerHandle, this, &ABattleBlasterGameMode::OnGameOverTimerTimeout, GameOverDelay, false);
	}
}

void ABattleBlasterGameMode::OnGameOverTimerTimeout()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance) {
		UBattleBlasterGameInstance* BattleBlasterGameInstance = Cast<UBattleBlasterGameInstance>(GameInstance);
		if (BattleBlasterGameInstance) {

			if (IsVictory) {
				BattleBlasterGameInstance->LoadNextLevel();
			}
			else {
				BattleBlasterGameInstance->RestartCurrentLevel();
			}
		}
	}
}


