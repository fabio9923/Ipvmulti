// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "IpvmultiCharacter.h"  
#include "IpvmultiGameMode.generated.h"

UCLASS(minimalapi)
class AIpvmultiGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AIpvmultiGameMode();

	UFUNCTION(BlueprintCallable)
	void CompleteMission(AIpvmultiCharacter* Player);

	virtual void RestartPlayer(AController* NewPlayer) override;

protected:
	UPROPERTY()
	TArray<AActor*> PlayerStarts;  // Cambiado a APlayerStart específico
};



