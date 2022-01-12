// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "TPSAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 *  Usual AttributeSet implementation. We'll be using Health, Damage and Points.
 */
UCLASS()
class TPS_API UTPSAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UTPSAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UTPSAttributeSet, Health)

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Damage)
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UTPSAttributeSet, Damage)

	UFUNCTION()
	virtual void OnRep_Damage(const FGameplayAttributeData& OldDamage);


	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Points)
	FGameplayAttributeData Points;
	ATTRIBUTE_ACCESSORS(UTPSAttributeSet, Points)

	UFUNCTION()
	virtual void OnRep_Points(const FGameplayAttributeData& OldPoints);
	
};
