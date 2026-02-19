// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "Tower.h"
#include "BattleBlasterGameMode.h"

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
}
