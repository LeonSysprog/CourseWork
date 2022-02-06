// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTChaseTaskNode.h"
#include "Kismet/GameplayStatics.h"
#include "AI/AIEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type UBTChaseTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIEnemyController * AICon = Cast<AAIEnemyController>(OwnerComp.GetAIOwner());

    if (AICon)
    {
        UBlackboardComponent * BlackboardComp = AICon->GetBlackboardComp();

        UAIBlueprintHelperLibrary * lib = NewObject<UAIBlueprintHelperLibrary>();

        if (lib)
        {
            lib->SimpleMoveToLocation(
                AICon,
                UGameplayStatics::GetPlayerController(this, 0)->GetCharacter()->GetActorLocation());

            return EBTNodeResult::Succeeded;
        }
    }

    return EBTNodeResult::Failed;
}

