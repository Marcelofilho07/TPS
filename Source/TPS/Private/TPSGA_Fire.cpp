// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSGA_Fire.h"
#include "TPS/TPSCharacter.h"
#include "AbilitySystemComponent.h"
#include "TPSAttributeSet.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "TPSPlayerController.h"
#include "Kismet/GameplayStatics.h"

/*This is where we handle shooting and points calculation*/

UTPSGA_Fire::UTPSGA_Fire()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AbilityInputID = ETPSAbilityInputID::Fire;
}

void UTPSGA_Fire::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) //check for commit ability
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	if (!HasAuthority(&ActivationInfo)) //check if user has authority
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	FHitResult Hit;
	const float WeaponRange = 5000.f;
	if (ATPSCharacter* TPSCInstigator = Cast<ATPSCharacter>(ActorInfo->AvatarActor.Get())) //We'll use the instigador later to add points and handle visual effects
	{
		const FVector StartTrace = TPSCInstigator->GetFollowCamera()->GetComponentLocation();
		const FVector EndTrace = (TPSCInstigator->GetFollowCamera()->GetForwardVector() * WeaponRange) + StartTrace;

		FCollisionQueryParams QueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(WeaponTrace), false, ActorInfo->AvatarActor.Get());
		if (GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_Visibility, QueryParams))
		{
			FString ActName = Hit.GetActor()->GetName();
			if (ATPSCharacter* TPSC = Cast<ATPSCharacter>(Hit.GetActor()))
			{
				FGameplayEffectContextHandle EffectContext = TPSC->GetAbilitySystemComponent()->MakeEffectContext(); //Create Effect Context
				EffectContext.AddSourceObject(this);

				FGameplayEffectSpecHandle SpecHandle = TPSC->GetAbilitySystemComponent()->MakeOutgoingSpec(DamageAttributeEffect, 1, EffectContext);

				SpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), TPSCInstigator->GetAttributeSet()->GetDamage()); //Set Damage tag by SetByCaller using Damage Attribute

				if (SpecHandle.IsValid())
				{
					FActiveGameplayEffectHandle GEHandle = TPSC->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get()); //Apply Effect
					if (TPSC->GetAttributeSet()->GetHealth() <= 0) // Give points to Instigator if target reached 0 life
					{

						FGameplayEffectContextHandle EffectContextInstigator = TPSCInstigator->GetAbilitySystemComponent()->MakeEffectContext(); //Create Effect Context
						EffectContextInstigator.AddSourceObject(this);

						FGameplayEffectSpecHandle SpecHandleInstigator = TPSCInstigator->GetAbilitySystemComponent()->MakeOutgoingSpec(PointsAttributeEffect, 1, EffectContextInstigator);

						if (SpecHandleInstigator.IsValid())
						{
							FActiveGameplayEffectHandle GEHandleInstigator = TPSCInstigator->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandleInstigator.Data.Get()); //Apply Effect
						}
					}
				}

			}



			if (TPSCInstigator->ImpactParticles) //If we hit something we spawn impact particles
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TPSCInstigator->ImpactParticles, FTransform(Hit.ImpactNormal.Rotation(), Hit.ImpactPoint));
			}
		}

		if (TPSCInstigator->MuzzleParticles) //always spawn muzzle particles
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TPSCInstigator->MuzzleParticles, TPSCInstigator->FP_Gun->GetSocketTransform(FName("Muzzle")));
		}

		if (TPSCInstigator->FireSound != nullptr) //and always play sound effect
		{
			UGameplayStatics::PlaySoundAtLocation(this, TPSCInstigator->FireSound, TPSCInstigator->GetActorLocation());
		}
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
