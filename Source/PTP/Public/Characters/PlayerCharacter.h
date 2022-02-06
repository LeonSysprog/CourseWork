// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Characters/MainCharacter.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "CoreMinimal.h"
#include "Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Hiders/Locker.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class PTP_API APlayerCharacter : public AMainCharacter
{
    GENERATED_BODY()

  public:
    APlayerCharacter();

    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = Camera,
              meta     = (AllowPrivateAccess = "true"))
    USpringArmComponent * CameraBoom;

    UFUNCTION(BlueprintCallable)
    void GrabRagdoll();

    UFUNCTION(BlueprintCallable)
    void SetStealth();

    UFUNCTION(BlueprintCallable)
    bool GetStealth();

    UFUNCTION(BlueprintCallable)
    bool GetGrabStatus();

    UFUNCTION(BlueprintCallable)
    bool GetSwitchState();

    UPROPERTY(EditAnywhere, Category = Hear)
    float StepVolume = 1.f;

    // Component is used to emit sounds to AIPatrol
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UPawnNoiseEmitterComponent * PawnNoiseEmitterComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UPawnSensingComponent * PawnSensingComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UPhysicsConstraintComponent * PhysicsConstraintCompLeft;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UPhysicsConstraintComponent * PhysicsConstraintCompRight;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UPhysicsConstraintComponent * PhysicsConstraintCompUpperArm;

    void SelfSimulate();

    // check() and ensuremsgf() for PLATFORM_BREAKPOINT()
    // Disable capsule component, widget, controller and detach body
    UFUNCTION(BlueprintCallable)
    void EnemySimulate(APawn * pawn);

  protected:
    bool StealthStatus = false;
    bool IsGrabbed     = false;
    bool IsSwitchState = true;
    bool IsLocker      = false;
    bool InHaven       = false;

    UAnimSequence *           animation;
    FAnimNotifyEventReference AnimNotify;

    class ALocker * locker;

    UPROPERTY(BlueprintReadWrite)
    ACharacter * DragEnemy;

    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = Camera,
              meta     = (AllowPrivateAccess = "true"))
    UCameraComponent * FollowCamera;

    virtual void SetupPlayerInputComponent(class UInputComponent * PlayerInputComponent) override;

    void MoveForward(float Value);

    void MoveRight(float Value);

    void BeginPlay() override;

    void Tick(float DeltaSeconds) override;

  public:
    UFUNCTION(BlueprintCallable)
    void AnimNotify_GrabToWalk();

    UFUNCTION(BlueprintCallable)
    void Kill();

    UFUNCTION(BlueprintCallable)
    void SetLockerStatus(bool status);

    UFUNCTION(BlueprintCallable)
    bool GetLockerStatus();

    UFUNCTION(BlueprintCallable)
    void HideEnemy();

    UFUNCTION(BlueprintCallable)
    APawn * GetDragEnemy();

	UFUNCTION(BlueprintCallable)
    bool GetHavenStatus();

	UFUNCTION(BlueprintCallable)
    void SetHavenStatus();
};
