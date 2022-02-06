// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTSelectPatrolPoint.h"
#include "AI/AIEnemyController.h"
#include "AI/AIPatrolPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UBTSelectPatrolPoint::ExecuteTask(UBehaviorTreeComponent & OwnerComp,
                                                      uint8 *                  NodeMemory)
{
    AAIEnemyController * AICon = Cast<AAIEnemyController>(OwnerComp.GetAIOwner());

    if (AICon)
    {
        /*Get BB component*/
        UBlackboardComponent * BlackboardComp = AICon->GetBlackboardComp();

        AAIPatrolPoint * CurrentPoint =
            Cast<AAIPatrolPoint>(BlackboardComp->GetValueAsObject("LocationToGo"));

        TArray<AActor *> AvailablePatrolPoints = AICon->GetPatrolPoints();

        AAIPatrolPoint * NextPatrolPoint = nullptr;

        if (AICon->CurrentPatrolPoint != AvailablePatrolPoints.Num() - 1)
        {
            NextPatrolPoint =
                Cast<AAIPatrolPoint>(AvailablePatrolPoints[++AICon->CurrentPatrolPoint]);
        }
        else    // if there is not any more points to go to
        {
            NextPatrolPoint           = Cast<AAIPatrolPoint>(AvailablePatrolPoints[0]);
            AICon->CurrentPatrolPoint = 0;
        }

        AEnemyCharacter * Patrol = Cast<AEnemyCharacter>(AICon->GetCharacter());

        // Interrupt of MoveTo task node
    	
    	if (AICon->GetDetectFlag())
    	{
            BlackboardComp->SetValueAsVector("LocationToGo", UGameplayStatics::GetPlayerController(this, 0)->GetCharacter()->GetActorLocation());
    	}
        else
        {
            BlackboardComp->SetValueAsVector("LocationToGo", NextPatrolPoint->GetActorLocation());
        }

        BlackboardComp->SetValueAsBool("DetectFlag", AICon->GetDetectFlag());

        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}
