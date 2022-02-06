// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "CoreMinimal.h"
#include "AIEnemyController.generated.h"

/**
 *
 */
UCLASS()
class PTP_API AAIEnemyController : public AAIController
{
    GENERATED_BODY()

    /* Behavior tree component*/
    UBehaviorTreeComponent * BehaviorComp;

    /* Our blackboard component*/
    UBlackboardComponent * BlackboardComp;

    /*Blackborad keys*/
    UPROPERTY(EditAnywhere, Category = AI)
    FName LocationToGoKey;

    UPROPERTY(EditDefaultsOnly, Category = AI)
    FName PlayerKey;

    TArray<AActor *> PatrolPoints;

    virtual void OnPossess(APawn * pawn) override;

  public:
    AAIEnemyController();

    bool DetectFlag;

    int32 CurrentPatrolPoint;

    float StepVolume = 1.f;

    bool GetDetectFlag();
    void SetDetectFlag(bool value);

    /*Inline getter functions*/

    FORCEINLINE UBlackboardComponent * GetBlackboardComp() const { return BlackboardComp; }
    FORCEINLINE TArray<AActor *> GetPatrolPoints() const { return PatrolPoints; }

    UBehaviorTreeComponent * GetBehaviorTreeComponent();
};
