// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoPickup.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h" 
#include "Ipvmulti/Ipvmulti.h"
#include "Ipvmulti/IpvmultiCharacter.h"

// Sets default values
AAmmoPickup::AAmmoPickup()
{
	PrimaryActorTick.bCanEverTick = false;
    
	// Enable replication
	bReplicates = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->InitSphereRadius(50.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AAmmoPickup::BeginPlay()
{
	Super::BeginPlay();
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AAmmoPickup::OnOverlapBegin);
}

void AAmmoPickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AIpvmultiCharacter* PlayerCharacter = Cast<AIpvmultiCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		// Only the server handles pickup logic
		if (GetLocalRole() == ROLE_Authority)
		{
			PlayerCharacter->AddAmmo(AmmoToAdd);
			Destroy(); // Destroy on server (will replicate to clients)
		}
		else
		{
			// If it's a client, ask the server to destroy the pickup
			Server_DestroyPickup();
		}
	}
}

// Server RPC Implementation
void AAmmoPickup::Server_DestroyPickup_Implementation()
{
	// The server destroys the pickup (replicates to all clients)
	Destroy();
}


bool AAmmoPickup::Server_DestroyPickup_Validate()
{
	return true;
}

