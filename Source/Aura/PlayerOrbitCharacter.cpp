#include "PlayerOrbitCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"

APlayerOrbitCharacter::APlayerOrbitCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 600.f;
    CameraBoom->bUsePawnControlRotation = false;
    CameraBoom->bInheritPitch = false;
    CameraBoom->bInheritYaw = false;
    CameraBoom->bInheritRoll = false;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;
}

void APlayerOrbitCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
            {
                if (InputMapping)
                {
                    Subsystem->AddMappingContext(InputMapping, 0);
                }
            }
        }
    }

    const FRotator InitialRotation = CameraBoom->GetComponentRotation();
    CurrentPitch = InitialRotation.Pitch;
    CurrentYaw = InitialRotation.Yaw;
}

void APlayerOrbitCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (OrbitHoldAction)
        {
            EnhancedInputComponent->BindAction(OrbitHoldAction, ETriggerEvent::Started, this, &APlayerOrbitCharacter::StartOrbiting);
            EnhancedInputComponent->BindAction(OrbitHoldAction, ETriggerEvent::Completed, this, &APlayerOrbitCharacter::StopOrbiting);
            EnhancedInputComponent->BindAction(OrbitHoldAction, ETriggerEvent::Canceled, this, &APlayerOrbitCharacter::StopOrbiting);
        }

        if (MouseDeltaAction)
        {
            EnhancedInputComponent->BindAction(MouseDeltaAction, ETriggerEvent::Triggered, this, &APlayerOrbitCharacter::HandleMouseDelta);
        }
    }
}

void APlayerOrbitCharacter::StartOrbiting()
{
    bOrbiting = true;
}

void APlayerOrbitCharacter::StopOrbiting()
{
    bOrbiting = false;
}

void APlayerOrbitCharacter::HandleMouseDelta(const FInputActionValue& Value)
{
    if (!bOrbiting)
    {
        return;
    }

    const FVector2D MouseDelta = Value.Get<FVector2D>();
    ApplyCameraRotation(MouseDelta);
}

void APlayerOrbitCharacter::ApplyCameraRotation(const FVector2D& MouseDelta)
{
    CurrentYaw += MouseDelta.X;
    CurrentPitch = FMath::Clamp(CurrentPitch + MouseDelta.Y, -80.f, 80.f);

    const FRotator NewRotation(CurrentPitch, CurrentYaw, 0.f);
    CameraBoom->SetWorldRotation(NewRotation);
}
