// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPSCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "TPSAttributeSet.h"
#include "TPSGameplayAbility.h"
#include "TPSAbilitySystemComponent.h"
#include "TPSPlayerState.h"
#include "TPSPlayerController.h"
#include "TPS.h"
#include "TPSGameModeBase.h"
#include <GameplayEffectTypes.h>

//////////////////////////////////////////////////////////////////////////
// ATPSCharacter

ATPSCharacter::ATPSCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;


	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.73f, 22.64f, 11.1f));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	//if (ATPSGameModeBase* GM = Cast<ATPSGameModeBase>(GetWorld()->GetAuthGameMode()))
	//	GM->PlayersInLobby.Add(this);

	FP_Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), TEXT("index_01_r"));
	if (ATPSPlayerState* PS = Cast<ATPSPlayerState>(GetPlayerState()))
		PS->ToggleInGame();
}

UAbilitySystemComponent* ATPSCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

UTPSAttributeSet* ATPSCharacter::GetAttributeSet() const
{
	return AttributeSetBase.Get();
}

void ATPSCharacter::InitializeAttributes()
{
	if (AbilitySystemComponent.IsValid() && DefaultAttributeEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContext);

		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void ATPSCharacter::GiveAbilities()
{
	if (HasAuthority() && AbilitySystemComponent.IsValid())
	{
		for (TSubclassOf<UTPSGameplayAbility>& StartupAbility : DefaultAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
		}
	}
}

void ATPSCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	ATPSPlayerState* PS = GetPlayerState<ATPSPlayerState>();
	if (PS)
	{
		// Set the ASC on the Server. Clients do this in OnRep_PlayerState()
		AbilitySystemComponent = Cast<UTPSAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

		AttributeSetBase = PS->GetAttributeSet();

		InitializeAttributes();
		GiveAbilities();

		ATPSPlayerController* PC = Cast<ATPSPlayerController>(GetController());
		if (PC)
		{
			PC->CreateHUD();
		}

	}

}

void ATPSCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	ATPSPlayerState* PS = GetPlayerState<ATPSPlayerState>();
	if (PS)
	{
		AbilitySystemComponent = Cast<UTPSAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		AbilitySystemComponent->InitAbilityActorInfo(PS, this);

		BindASCInput();

		AttributeSetBase = PS->GetAttributeSet();

		InitializeAttributes();

		ATPSPlayerController* PC = Cast<ATPSPlayerController>(GetController());
		if (PC)
		{
			PC->CreateHUD();
		}

	}

	BindASCInput();
}

void ATPSCharacter::Die()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		ATPSGameModeBase* GM = Cast<ATPSGameModeBase>(GetWorld()->GetAuthGameMode());

		if (GM)
		{
			GM->HeroDied(GetController());
		}
	}

	Destroy();
}

void ATPSCharacter::BindASCInput()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (ASC && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds(
			"Confirm",
			"Cancel",
			"ETPSAbilityInputID",
			static_cast<int32>(ETPSAbilityInputID::Confirm),
			static_cast<int32>(ETPSAbilityInputID::Cancel));

		ASC->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATPSCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATPSCharacter::MoveRight);

	// "turn" handles devices that provide an absolute delta, such as a mouse.
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	BindASCInput();
}

void ATPSCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ATPSCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		AddMovementInput(GetActorRightVector(), Value);
	}
}
