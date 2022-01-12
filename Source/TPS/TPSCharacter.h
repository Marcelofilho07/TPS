// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "TPSCharacter.generated.h"

UCLASS(config=Game)
class ATPSCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USceneComponent* FP_MuzzleLocation;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	ATPSCharacter();

	virtual void BeginPlay();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual class UTPSAttributeSet* GetAttributeSet() const;

	virtual void InitializeAttributes();

	virtual void GiveAbilities();

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

	void Die();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TArray<TSubclassOf<class UTPSGameplayAbility>> DefaultAbilities;

	void BindASCInput();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* FireSound;

	/** Seconds between shots */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UParticleSystem* MuzzleParticles;

	/** Seconds between shots */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UParticleSystem* ImpactParticles;

protected:

	TWeakObjectPtr<class UTPSAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<class UTPSAttributeSet> AttributeSetBase;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

