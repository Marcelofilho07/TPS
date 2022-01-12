// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerSphere.h"
#include "TPSHealthPowerUp.generated.h"

/**
 * A simple trigger sphere. Binding OnActorBeginOverlap to a Gameplay Effect. Heals player.
 */
UCLASS()
class TPS_API ATPSHealthPowerUp : public ATriggerSphere
{
	GENERATED_BODY()

public:
	ATPSHealthPowerUp();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* VisualMesh;

	UFUNCTION()
	void Event(class AActor* OverlappedActor, class AActor* OtherActor);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UGameplayEffect> HealAttributeEffect;

protected:
	virtual void BeginPlay();
};
