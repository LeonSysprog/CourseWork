// Fill out your copyright notice in the Description page of Project Settings.


#include "Hiders/Locker.h"
#include "Characters/EnemyCharacter.h"

// Sets default values
ALocker::ALocker()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if
    // you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mesh"));

    PhysicsConstraintComp =
        CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraintComp"));
    PhysicsConstraintComp->SetupAttachment(mesh, "Socket");
}

// Called when the game starts or when spawned
void ALocker::BeginPlay()
{
    Super::BeginPlay();

    player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerController(this, 0)->GetCharacter());
}

// Called every frame
void ALocker::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    player->SetLockerStatus(player->PawnSensingComp->CouldSeePawn(this, false) ? true : false);
}

// Called to bind functionality to input
void ALocker::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ALocker::AttachEnemy()
{
    PhysicsConstraintComp->SetConstrainedComponents(
        mesh,
        "joint1",
        Cast<AEnemyCharacter>(player->GetDragEnemy())->GetMesh(),
        "spine_02");
    PhysicsConstraintComp->SetConstraintReferencePosition(EConstraintFrame::Frame2,
                                                          FVector::ZeroVector);
}
