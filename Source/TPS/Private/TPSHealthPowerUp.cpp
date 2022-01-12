// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSHealthPowerUp.h"
#include "TPS/TPSCharacter.h"
#include "AbilitySystemComponent.h"
#include "TPSAttributeSet.h"

ATPSHealthPowerUp::ATPSHealthPowerUp()
{
	bReplicates = true;
	OnActorBeginOverlap.AddDynamic(this, &ATPSHealthPowerUp::Event);

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	VisualMesh->SetOnlyOwnerSee(false);
	SetRootComponent(VisualMesh);
}

void ATPSHealthPowerUp::Event(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && OtherActor != this)
	{
		if (ATPSCharacter* TPSC = Cast<ATPSCharacter>(OtherActor))
		{
			FGameplayEffectContextHandle EffectContext = TPSC->GetAbilitySystemComponent()->MakeEffectContext(); //Create Effect Context
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle SpecHandle = TPSC->GetAbilitySystemComponent()->MakeOutgoingSpec(HealAttributeEffect, 1, EffectContext);

			if (SpecHandle.IsValid())
			{
				FActiveGameplayEffectHandle GEHandle = TPSC->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get()); //Apply Effect
				Destroy();
			}
		}
	}
}

void ATPSHealthPowerUp::BeginPlay()
{
	Super::BeginPlay();
}
