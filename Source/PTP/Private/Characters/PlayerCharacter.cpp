// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacter.h"
#include "AI/AIEnemyController.h"
#include "Camera/CameraComponent.h"
#include "Characters/EnemyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

APlayerCharacter::APlayerCharacter()

{
    PrimaryActorTick.bCanEverTick = true;

    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw   = false;
    bUseControllerRotationRoll  = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement =
        true;    // Character moves in the direction of input...
    GetCharacterMovement()->RotationRate =
        FRotator(0.0f, 540.0f, 0.0f);    // ...at this rotation rate
    GetCharacterMovement()->JumpZVelocity = 600.f;
    GetCharacterMovement()->AirControl    = 0.2f;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength         = 300.0f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    // Hear
    PawnNoiseEmitterComponent =
        CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("PawnNoiseEmitterComponent"));

    // See
    PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
    PawnSensingComp->SetPeripheralVisionAngle(35.f);

    PhysicsConstraintCompLeft =
        CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraintCompLeft"));
    PhysicsConstraintCompLeft->SetupAttachment(GetMesh(), "hand_lSocket");

    PhysicsConstraintCompRight =
        CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraintCompRight"));
    PhysicsConstraintCompRight->SetupAttachment(GetMesh(), "hand_rSocket");

    PhysicsConstraintCompUpperArm =
        CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraintCompUpperArm"));
    PhysicsConstraintCompUpperArm->SetupAttachment(GetMesh(), "upperarm_lSocket");
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    check(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

    PlayerInputComponent->BindAction("Stealth", IE_Pressed, this, &APlayerCharacter::SetStealth);
    PlayerInputComponent->BindAction("SelfSimulate",
                                     IE_Pressed,
                                     this,
                                     &APlayerCharacter::SelfSimulate);
    PlayerInputComponent->BindAction("GrabRagdoll",
                                     IE_Pressed,
                                     this,
                                     &APlayerCharacter::GrabRagdoll);
    PlayerInputComponent->BindAction("Kill", IE_Pressed, this, &APlayerCharacter::Kill);
    PlayerInputComponent->BindAction("HideEnemy", IE_Pressed, this, &APlayerCharacter::HideEnemy);
}

void APlayerCharacter::MoveForward(float Value)
{
    if ((Controller == nullptr) && (Value == 0.0f))
    {
        return;
    }

    // find out which way is forward
    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);

    // get forward vector
    const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    AddMovementInput(Direction, Value);

    PawnNoiseEmitterComponent->MakeNoise(this, StepVolume, GetActorLocation());
}

void APlayerCharacter::MoveRight(float Value)
{
    if ((Controller == nullptr) && (Value == 0.0f))
    {
        return;
    }

    // find out which way is right
    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);

    // get right vector
    const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    // add movement in that direction
    AddMovementInput(Direction, Value);

    PawnNoiseEmitterComponent->MakeNoise(this, StepVolume, GetActorLocation());
}

void APlayerCharacter::SetStealth()
{
    StealthStatus = !StealthStatus;
    StepVolume    = 0.f;

    if (StealthStatus)
    {
        GetCharacterMovement()->MaxWalkSpeed = 300;
    }
    else
    {
        GetCharacterMovement()->MaxWalkSpeed = 600;
    }
}

bool APlayerCharacter::GetStealth() { return StealthStatus; }

void APlayerCharacter::SelfSimulate() { GetMesh()->SetSimulatePhysics(true); }

void APlayerCharacter::EnemySimulate(APawn * pawn)
{
    if (IsLocker)
    {
        locker = Cast<ALocker>(pawn);
        UE_LOG(LogTemp, Error, TEXT("IsLocker"));
    }
    else
    {
        AEnemyCharacter * Enemy = Cast<AEnemyCharacter>(pawn);

        if (GetStealth() && !(Enemy->IsDetected))
        {
            DragEnemy = Enemy;
        }
    }
}

void APlayerCharacter::GrabRagdoll()
{
    if (!DragEnemy)
    {
        UE_LOG(LogTemp, Error, TEXT("DragEnemy is nullptr"));
        return;
    }

    if (IsGrabbed)
    {
        PhysicsConstraintCompUpperArm->BreakConstraint();
        IsGrabbed = false;
        // DragEnemy = nullptr;
        return;
    }

    IsGrabbed     = true;
    IsSwitchState = true;

    PhysicsConstraintCompLeft->SetConstrainedComponents(GetMesh(),
                                                        "hand_l",
                                                        DragEnemy->GetMesh(),
                                                        "hand_r");
    PhysicsConstraintCompLeft->SetConstraintReferencePosition(EConstraintFrame::Frame2,
                                                              FVector::ZeroVector);

    PhysicsConstraintCompRight->SetConstrainedComponents(GetMesh(),
                                                         "hand_r",
                                                         DragEnemy->GetMesh(),
                                                         "hand_l");
    PhysicsConstraintCompRight->SetConstraintReferencePosition(EConstraintFrame::Frame2,
                                                               FVector::ZeroVector);
}

bool APlayerCharacter::GetGrabStatus() { return IsGrabbed; }

bool APlayerCharacter::GetSwitchState() { return IsSwitchState; }

void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    // Use UFUNCTION(BlueprintCallable) for binding delegate
    PawnSensingComp->OnSeePawn.AddDynamic(this, &APlayerCharacter::EnemySimulate);

    PhysicsConstraintCompLeft->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
    PhysicsConstraintCompLeft->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
    PhysicsConstraintCompLeft->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0.f);

    PhysicsConstraintCompLeft->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.f);
    PhysicsConstraintCompLeft->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.f);
    PhysicsConstraintCompLeft->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.f);

    PhysicsConstraintCompRight->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
    PhysicsConstraintCompRight->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
    PhysicsConstraintCompRight->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0.f);

    PhysicsConstraintCompRight->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.f);
    PhysicsConstraintCompRight->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.f);
    PhysicsConstraintCompRight->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.f);

    PhysicsConstraintCompUpperArm->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
    PhysicsConstraintCompUpperArm->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
    PhysicsConstraintCompUpperArm->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0.f);

    PhysicsConstraintCompUpperArm->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.f);
    PhysicsConstraintCompUpperArm->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.f);
    PhysicsConstraintCompUpperArm->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.f);

    // Animation Notify

    GetMesh()->GetAnimInstance()->AddExternalNotifyHandler(this, "AnimNotify_GrabToWalk");
}

TArray<FAnimNotifyEventReference> ActiveNotifies;

void APlayerCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void APlayerCharacter::AnimNotify_GrabToWalk()
{
    PhysicsConstraintCompLeft->BreakConstraint();
    PhysicsConstraintCompRight->BreakConstraint();

    PhysicsConstraintCompUpperArm->SetConstrainedComponents(GetMesh(),
                                                            "upperarm_l",
                                                            DragEnemy->GetMesh(),
                                                            "spine_02");
    PhysicsConstraintCompUpperArm->SetConstraintReferencePosition(EConstraintFrame::Frame2,
                                                                  FVector::ZeroVector);

    IsSwitchState = false;

    if (!IsSwitchState)
    {
        UE_LOG(LogTemp, Error, TEXT("GrabToWalk"));
    }
}

void APlayerCharacter::Kill()
{
    AEnemyCharacter * Enemy = Cast<AEnemyCharacter>(DragEnemy);

    if (Enemy && !Enemy->GetDeadStatus())
    {
        Enemy->IsDead = true;

        // Enemy->SetActorLocation(GetActorLocation(), false, nullptr, ETeleportType::None);
        Enemy->SetActorRotation(GetActorRotation(), ETeleportType::None);
        Enemy->GetMesh()->SetSimulatePhysics(true);
        // Enemy->GetCharacterMovement()->DisableMovement();

        // Disable CapsuleComponent
        Enemy->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

        // Disable Widget
        Enemy->WidgetComp->DestroyComponent(false);

        // Disable Tick()
        Enemy->SetActorTickEnabled(false);

        // Disable AIEnemyController
        Enemy->GetController()->UnPossess();
    }
}

void APlayerCharacter::SetLockerStatus(bool status) { IsLocker = status; }

bool APlayerCharacter::GetLockerStatus() { return IsLocker; }

APawn * APlayerCharacter::GetDragEnemy() { return DragEnemy; }

void APlayerCharacter::HideEnemy()
{
    if (DragEnemy && locker)
    {
        PhysicsConstraintCompUpperArm->BreakConstraint();
        locker->AttachEnemy();
    }
}

bool APlayerCharacter::GetHavenStatus() { return InHaven; }

void APlayerCharacter::SetHavenStatus() { InHaven = !InHaven; }
