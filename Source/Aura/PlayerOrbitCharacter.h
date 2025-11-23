#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerOrbitCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UInputMappingContext;

/**
 * Character with a spring arm + camera that supports orbiting while holding the left mouse button.
 */
UCLASS()
class AURA_API APlayerOrbitCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    APlayerOrbitCharacter();

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
    void StartOrbiting();
    void StopOrbiting();
    void HandleMouseDelta(const FInputActionValue& Value);

    void ApplyCameraRotation(const FVector2D& MouseDelta);

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UCameraComponent> FollowCamera;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputMappingContext> InputMapping;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> OrbitHoldAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> MouseDeltaAction;

    bool bOrbiting = false;
    float CurrentPitch = 0.f;
    float CurrentYaw = 0.f;
};
