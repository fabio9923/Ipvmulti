// Copyright Epic Games, Inc. All Rights Reserved.

#include "IpvmultiGameMode.h"
#include "IpvmultiCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"  // Añade este include

AIpvmultiGameMode::AIpvmultiGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AIpvmultiGameMode::CompleteMission(AIpvmultiCharacter* Player)
{
	if (Player)
	{
		// Implementa tu lógica de misión completada aquí
		UE_LOG(LogTemp, Log, TEXT("Mission completed by %s"), *Player->GetName());
	}
}

void AIpvmultiGameMode::RestartPlayer(AController* NewPlayer)
{
	if(!NewPlayer || !NewPlayer->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Controller in RestartPlayer!"));
		return;
	}

	// Destruir el pawn actual si existe
	if(APawn* OldPawn = NewPlayer->GetPawn())
	{
		OldPawn->Destroy();
	}

	// Buscar PlayerStart
	TArray<AActor*> Starts;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), Starts);

	AActor* Start = nullptr;
	if(Starts.Num() > 0)
	{
		Start = Starts[FMath::RandRange(0, Starts.Num()-1)];
	}

	// Crear nuevo pawn
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = NewPlayer;
	SpawnParams.Instigator = NewPlayer->GetInstigator();
    
	APawn* NewPawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, 
		Start ? Start->GetActorLocation() : FVector::ZeroVector,
		Start ? Start->GetActorRotation() : FRotator::ZeroRotator,
		SpawnParams);

	if(NewPawn)
	{
		NewPlayer->Possess(NewPawn);
		UE_LOG(LogTemp, Warning, TEXT("Respawn successful at %s"), 
			*NewPawn->GetActorLocation().ToString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn new pawn!"));
	}
}
