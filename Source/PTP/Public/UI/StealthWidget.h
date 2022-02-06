// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "StealthWidget.generated.h"

/**
 *
 */
UCLASS()
class PTP_API UStealthWidget : public UUserWidget
{
    GENERATED_BODY()

  public:
    virtual bool Initialize() override;

    UPROPERTY(EditAnywhere, meta = (BindWidget), Category = UI)
    class UButton * StealthButton;
};
