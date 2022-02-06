// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnemyCharacter.h"
#include "AI/AIEnemyController.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"
#include "UI/StealthWidget.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance
    // if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Initialize senses
    PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
    PawnSensingComp->SetPeripheralVisionAngle(73.075134);

    // Set speed
    UCharacterMovementComponent * Speed =
        Cast<UCharacterMovementComponent>(this->GetCharacterMovement());
    Speed->MaxWalkSpeed = 300;


    // Set traffic lights
    WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
    WidgetComp->AttachTo(this->GetRootComponent());
    WidgetComp->SetRelativeLocation(FVector(4.820395, -0.35376, 117.267693));
}

void AEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();

    SWidget = NewObject<UStealthWidget>(StealthWidget);
    WidgetComp->SetTwoSided(true);
    PlayerCharacter =
        Cast<APlayerCharacter>(UGameplayStatics::GetPlayerController(this, 0)->GetCharacter());
    PawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemyCharacter::SeePlayerCharacter);
    PawnSensingComp->OnHearNoise.AddDynamic(this, &AEnemyCharacter::HearPlayerCharacter);
}

void AEnemyCharacter::Tick(float DeltaSeconds)
{
    Cast<AAIEnemyController>(GetController())->SetDetectFlag(IsDetected);

    if (WidgetComp && !IsDetected)
    {
        UE_LOG(LogTemp, Error, TEXT("Player is detected"));
        WidgetComp->SetBackgroundColor(FLinearColor::Green);
        WidgetComp->SetTintColorAndOpacity(FLinearColor::Green);
    }
}

void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

bool AEnemyCharacter::GetDeadStatus() { return IsDead; }

void AEnemyCharacter::SeePlayerCharacter(APawn * pawn)
{
    if (PlayerCharacter->GetHavenStatus())
        return;

    IsDetected = true;
    UE_LOG(LogTemp, Error, TEXT("Player is detected"));
    WidgetComp->SetBackgroundColor(FLinearColor::Red);
    WidgetComp->SetTintColorAndOpacity(FLinearColor::Red);
	
    Cast<AAIEnemyController>(GetController())
        ->GetBlackboardComp()->SetValueAsVector("LocationToGo", PlayerCharacter->GetActorLocation());

	Cast<AAIEnemyController>(GetController())->GetBehaviorTreeComponent()->RestartTree();
}

void AEnemyCharacter::HearPlayerCharacter(APawn * pawn, const FVector & vector, float volume)
{
    IsDetected = true;
    UE_LOG(LogTemp, Error, TEXT("Player is heard"));
    WidgetComp->SetBackgroundColor(FLinearColor::Red);
    WidgetComp->SetTintColorAndOpacity(FLinearColor::Red);
}
