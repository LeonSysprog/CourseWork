// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "CoreMinimal.h"
#include "BTSelectPatrolPoint.generated.h"

/**
 *
 */
UCLASS()
class PTP_API UBTSelectPatrolPoint : public UBTTaskNode
{
    GENERATED_BODY()

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent & OwnerComp,
                                            uint8 *                  NodeMemory) override;
};
