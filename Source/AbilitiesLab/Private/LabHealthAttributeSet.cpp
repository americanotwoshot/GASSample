// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitiesLab/Public/LabHealthAttributeSet.h"
#include "AbilitiesLab/AbilitiesLab.h"
#include "GameplayEffectExtension.h"

ULabHealthAttributeSet::ULabHealthAttributeSet()
{
	InitHealth(100.0f);
	InitMaxHealth(100.0f);
}

void ULabHealthAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	UE_LOG(LogTemp, Warning, TEXT("PreChange: Attribute '%s'"), *Attribute.AttributeName);
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	
	Super::PreAttributeChange(Attribute, NewValue);
}

void ULabHealthAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	UE_LOG(LogTemp, Warning, TEXT("PostChange: Attribute '%s' changed %.2f -> %.2f"), *Attribute.AttributeName, OldValue, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		OnHealthChanged.Broadcast(this, OldValue, NewValue);
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		const float CurrentHealth = GetHealth();
		OnHealthChanged.Broadcast(this, CurrentHealth, CurrentHealth);
	}
}

void ULabHealthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	UE_LOG(LogTemp, Warning, TEXT("PostApply: Gameplay effect '%s' effect"), *Data.EffectSpec.Def->GetClass()->GetName());
	
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		const float DamageValue = GetDamage();
		const float OldHealthValue = GetHealth();
		const float MaxHealthValue = GetMaxHealth();
		const float NewHealthValue = FMath::Clamp(OldHealthValue - DamageValue, 0.0f, MaxHealthValue);

		if (OldHealthValue != NewHealthValue)
		{
			// Set the new health
			SetHealth(NewHealthValue);

			// calculate 'actual' damage applied
			const float DamageNumber = OldHealthValue - NewHealthValue;
			if (UAbilitySystemComponent* OwningAbilitySystemComponent = GetValid(GetOwningAbilitySystemComponent()))
			{
				// Broadcast a 'damage number' gameplay cue on the owning actor.
				const FGameplayTag DamageCueTag = FGameplayTag::RequestGameplayTag(FName("GameplayCue.DamageNumber"), true);
				FGameplayCueParameters DamageCueParams;
				DamageCueParams.NormalizedMagnitude = 1.f;
				DamageCueParams.RawMagnitude = DamageNumber;
				OwningAbilitySystemComponent->ExecuteGameplayCue(DamageCueTag, DamageCueParams);
			}
		}

		SetDamage(0.0f);
	}
	
}
