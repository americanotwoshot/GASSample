#pragma once

#include "CoreMinimal.h"
#include "MyAbilitySlotsEnum.generated.h"

UENUM(BlueprintType)
enum class EMyAbilitySlotsEnum : uint8
{
	PrimaryAbility     UMETA(DisplayName = "PrimaryAbility"),
	SecondaryAbility   UMETA(DisplayName = "SecondaryAbility"),
	UltimateAbility    UMETA(DisplayName = "UltimateAbility")
};