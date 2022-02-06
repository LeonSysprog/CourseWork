// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/AIPatrolPoint.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Characters/NPSCharacter.h"
#include "Characters/PlayerCharacter.h"
#include "Components/WidgetComponent.h"
#include "PlayerCharacter.h"
#include "UI/StealthWidget.h"
#include "EnemyCharacter.generated.h"

/**
 *
 */
UCLASS()
class PTP_API AEnemyCharacter : public ANPSCharacter
{
    GENERATED_BODY()

  public:
    // Sets default values for this character's properties
    AEnemyCharacter();

  protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

  public:
    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent * PlayerInputComponent) override;

    // Called every frame
    virtual void Tick(float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, Category = AI)
    UBehaviorTree * BehaviorTree;

    UPROPERTY(VisibleAnywhere, Category = AI)
    UPawnSensingComponent * PawnSensingComp;

    UPROPERTY(EditAnywhere, Category = AI)
    TSubclassOf<AAIPatrolPoint> route;

    UPROPERTY(EditAnywhere, Category = UI)
    TSubclassOf<UStealthWidget> StealthWidget;

    bool IsDetected = false;
    bool IsDead     = false;

    UStealthWidget * SWidget;

    UPROPERTY(EditAnywhere, Category = UI)
    UWidgetComponent * WidgetComp;

    UFUNCTION(BlueprintCallable)
    bool GetDeadStatus();

    UFUNCTION(BlueprintCallable)
    void SeePlayerCharacter(APawn * pawn);

    UFUNCTION(BlueprintCallable)
    void HearPlayerCharacter(APawn * pawn, const FVector & vector, float volume);

  private:
    class APlayerCharacter * PlayerCharacter;
    void                     DetectPlayer();
};