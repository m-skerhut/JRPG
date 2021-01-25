#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DataContainer.h"
#include "JRPG_BaseAbility.generated.h"


UENUM(BlueprintType)
enum class EAbilityRange : uint8
{
	MELEE,
	DISTANCE
};

UENUM(BlueprintType)
enum class EMagicType : uint8
{
	NONE  UMETA(DisplayName = ""),
	WHITE_MAGIC UMETA(DisplayName = "White Magic"),
	BLACK_MAGIC UMETA(DisplayName = "Black Magic")
};

UCLASS(Abstract, Blueprintable, BlueprintType)
class JRPG_API UJRPG_BaseAbility : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void UpgradeAbility();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString abilityName;				 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 abilityDamage;				 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float abilityManaCost;				 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 abilityLevel = 1;				 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 abilityMaxLevel;		
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 abilityUnlockCost;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EMagicType abilityType;			 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EElementalType abilityElementalType; 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAbilityRange abilityRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
	class UParticleSystem* weaponEffects;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
	class UParticleSystem* abilityEffects;
};
