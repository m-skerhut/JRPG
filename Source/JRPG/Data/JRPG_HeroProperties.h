#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DataContainer.h"
#include "JRPG_HeroProperties.generated.h"


UCLASS(Abstract, Blueprintable, BlueprintType)
class JRPG_API UJRPG_HeroProperties : public UObject
{
	GENERATED_BODY()

private:



public:
	//base hero stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AJRPG_BaseHero>  HeroClass;//this is the class we use in battle to operate the hero
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString heroName;
	//hp
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroStats")
	float baseHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroStats")
	float maxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroStats")
	float currentHealth;
	//mp
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroStats")
	float baseMana;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroStats")
	float maxMana;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroStats")
	float currentMana;
	//attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroStats")
	float physicalAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroStats")
	float currPhysicalAttack;
	//defense
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroStats")
	float physicalDefense;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroStats")
	float currPhysicalDefense;
	//magic
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroStats")
	float magicAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroStats")
	float currMagicAttack;
	//magic defense
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroStats")
	float magicDefense;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroStats")
	float currMagicDefense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroStats")
	float speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* InfoPic;
	//level
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroLevel")
	float currentXP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroLevel")
	float levelUpXP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroLevel")
	int32 currentLevel = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroLevel")
	int32 maxLevel = 99;
	//abilities
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroAbilities")
	int32 abilityPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroAbilities")
	TArray<TSubclassOf<class UJRPG_BaseAbility>> DefaultAbilityClasses;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroAbilities")
	TArray<TSubclassOf<class UJRPG_BaseAbility>> LockedAbilityClasses;
	//the abilities that are currently equipped
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HeroAbilities")
	TArray<class UJRPG_BaseAbility*> AbilityList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HeroEquippment")
	FBaseItemProperties equippedWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HeroEquippment")
	FBaseItemProperties equippedArmor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HeroEquippment")
	FBaseItemProperties equippedAmulet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HeroEquippment")
	FBaseItemProperties equippedRing;

	//called to initialize the properties at the start of the game
	void Initialize();

	void TakeDamage(float damageAmount);
	void RestoreHealth(float healAmount);
	void RestoreMana(float manaAmount);
	UFUNCTION(BlueprintCallable)
	void AddExperience(float xpAmount);
	UFUNCTION(BlueprintCallable)
	void LevelUp();
	void ChangeEquippment(FBaseItemProperties* item);
	void CalculateNewStats();
};
