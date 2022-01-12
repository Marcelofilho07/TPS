// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerSphere.h"
#include "TPSDoubleDamagePowerUp.generated.h"

/**
 * A simple trigger sphere. Binding OnActorBeginOverlap to a Gameplay Effect. Gives double damage.
 */
UCLASS()
class TPS_API ATPSDoubleDamagePowerUp : public ATriggerSphere
{
	GENERATED_BODY()

public:
	ATPSDoubleDamagePowerUp();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* VisualMesh;

	UFUNCTION()
	void Event(class AActor* OverlappedActor, class AActor* OtherActor);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UGameplayEffect> DoubleDamageAttributeEffect;

protected:
	virtual void BeginPlay();
};
