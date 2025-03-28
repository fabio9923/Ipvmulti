// Fill out your copyright notice in the Description page of Project Settings.


#include "Ipvmulti/Public/Actors/LaunchPad.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"


// Sets default values
ALaunchPad::ALaunchPad()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	OverlapComp = CreateDefaultSubobject<UBoxComponent>("BoxComp");
	RootComponent = OverlapComp;
	MeshComp->SetupAttachment(OverlapComp);
}

// Called when the game starts or when spawned
void ALaunchPad::BeginPlay()
{
	Super::BeginPlay();
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &ALaunchPad::OverLapLaunchPad);
}

void ALaunchPad::OverLapLaunchPad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ACharacter* MyCharacter = Cast<ACharacter>(OtherActor);
	if (MyCharacter)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Overlap"));
		}
	}
}

// Called every frame
void ALaunchPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

