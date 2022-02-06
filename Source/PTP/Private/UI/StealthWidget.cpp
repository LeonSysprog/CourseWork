// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StealthWidget.h"
#include "Components/Button.h"

bool UStealthWidget::Initialize()
{
    Super::Initialize();
    if (StealthButton)
    {
        StealthButton->SetColorAndOpacity(FLinearColor::Green);
    }
    return true;
}