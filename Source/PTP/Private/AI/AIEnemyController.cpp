// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIEnemyController.h"
#include "AI/AIPatrolPoint.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AAIEnemyController::AAIEnemyController()
{
    /*Initialize blackboard and behavior tree*/
    BehaviorComp   = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
    BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboradComp"));

    /*Initialize blackboard keys*/
    LocationToGoKey = "LocationToGo";
    PlayerKey       = "Target";
    DetectFlag      = false;

    CurrentPatrolPoint = 0;
}


void AAIEnemyController::OnPossess(APawn * pawn)
{
    Super::OnPossess(pawn);

    /*Get reference to the character*/
    AEnemyCharacter * AICharacter = Cast<AEnemyCharacter>(pawn);

    if (AICharacter)
    {
        if (AICharacter->BehaviorTree->BlackboardAsset)
        {
            BlackboardComp->InitializeBlackboard(*(AICharacter->BehaviorTree->BlackboardAsset));
        }

        /*Populate patrol point array*/
        UGameplayStatics::GetAllActorsOfClass(GetWorld(),
                                              Cast<AEnemyCharacter>(GetCharacter())->route,
                                              PatrolPoints);

        BehaviorComp->StartTree(*AICharacter->BehaviorTree);
    }
}

bool AAIEnemyController::GetDetectFlag() { return DetectFlag; }

void AAIEnemyController::SetDetectFlag(bool value) { DetectFlag = value; }

UBehaviorTreeComponent * AAIEnemyController::GetBehaviorTreeComponent() { return BehaviorComp; }
