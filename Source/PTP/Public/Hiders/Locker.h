// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "Characters/PlayerCharacter.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Locker.generated.h"

class APlayerCharacter;

UCLASS()
class PTP_API ALocker : public APawn
{
    GENERATED_BODY()

  public:
    // Sets default values for this pawn's properties
    ALocker();

	UPROPERTY(EditAnywhere, Category = StaticMeshComponent)
	UStaticMeshComponent * mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UPhysicsConstraintComponent * PhysicsConstraintComp;
	
  protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

  public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent * PlayerInputComponent) override;

    APlayerCharacter * player;

	UFUNCTION(BlueprintCallable)
    void AttachEnemy();
};
