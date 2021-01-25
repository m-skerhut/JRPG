#include "JRPG_HeroProperties.h"
#include "JRPG_BaseAbility.h"
#include "../Data/HelperFunctions.h"
#include "Kismet/GameplayStatics.h"

void UJRPG_HeroProperties::Initialize()
{
	for (int i = 0; i < DefaultAbilityClasses.Num(); i++)
	{
		UJRPG_BaseAbility* ability = NewObject<UJRPG_BaseAbility>(this, DefaultAbilityClasses[i]->GetFName(), RF_NoFlags, DefaultAbilityClasses[i].GetDefaultObject());
		if (ability)
		{
			AbilityList.Insert(ability, i);
		}
	}
	CalculateNewStats();
}

void UJRPG_HeroProperties::TakeDamage(float damageAmount)
{
	currentHealth = FMath::Clamp(currentHealth - damageAmount, 0.0f, maxHealth);
}

void UJRPG_HeroProperties::RestoreHealth(float healAmount)
{
	currentHealth = FMath::Clamp(currentHealth + healAmount, 0.0f, maxHealth);
}

void UJRPG_HeroProperties::RestoreMana(float manaAmount)
{
	currentMana = FMath::Clamp(currentMana + manaAmount, 0.0f, maxMana);
}

void UJRPG_HeroProperties::AddExperience(float xpAmount)
{

}

void UJRPG_HeroProperties::LevelUp()
{
	if (currentLevel < maxLevel)
	{
		float increaseValue = 1.25f;

		levelUpXP *= increaseValue;
		currentXP = 0;


		baseHealth *= increaseValue;
		baseMana *= increaseValue;
		physicalAttack *= increaseValue;
		physicalDefense *= increaseValue;
		magicAttack *= increaseValue;
		magicDefense *= increaseValue;

		currentLevel++;
		abilityPoints++;
		CalculateNewStats();

		currentHealth = maxHealth;
		currentMana = maxMana;

		//AddExperience(remainingXP);
	}
	else
	{
		currentXP = levelUpXP;
	}
}

void UJRPG_HeroProperties::ChangeEquippment(FBaseItemProperties* item)
{

	switch (item->itemType)
	{
	case EItemType::WEAPON:
		//if a weapon is already equipped we want to add it to inventory
		if (!equippedWeapon.itemRowName.IsNone())
		{
			FItemDynamicData oldEquippment;
			oldEquippment.dtRowName = equippedWeapon.itemRowName;
			oldEquippment.amount = 1;
			UHelperFunctions::AddItemToInventory(Cast<UJRPG_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld())), oldEquippment);
		}
		equippedWeapon = *item;

	break;
	case EItemType::ARMOR:
		//if armor is already equipped we want to add it to inventory
		if (!equippedArmor.itemRowName.IsNone())
		{
			FItemDynamicData oldEquippment;
			oldEquippment.dtRowName = equippedArmor.itemRowName;
			oldEquippment.amount = 1;
			UHelperFunctions::AddItemToInventory(Cast<UJRPG_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld())), oldEquippment);
		}
		equippedArmor = *item;
	break;
	case EItemType::AMULET:
		//if a amulet is already equipped we want to add it to inventory
		if (!equippedAmulet.itemRowName.IsNone())
		{
			FItemDynamicData oldEquippment;
			oldEquippment.dtRowName = equippedAmulet.itemRowName;
			oldEquippment.amount = 1;
			UHelperFunctions::AddItemToInventory(Cast<UJRPG_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld())), oldEquippment);
		}
		equippedAmulet = *item;
	break;
	case EItemType::RING:
		//if a ring is already equipped we want to add it to inventory
		if (!equippedRing.itemRowName.IsNone())
		{
			FItemDynamicData oldEquippment;
			oldEquippment.dtRowName = equippedRing.itemRowName;
			oldEquippment.amount = 1;
			UHelperFunctions::AddItemToInventory(Cast<UJRPG_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld())), oldEquippment);
		}
		equippedRing = *item;
	break;
	}
	//reset stats
	maxHealth = baseHealth;
	maxMana = baseMana;
	currPhysicalAttack = physicalAttack;
	currPhysicalDefense = physicalDefense;
	currMagicAttack = magicAttack;
	currMagicDefense = magicDefense;


	//calculate new stats
	CalculateNewStats();
}

void UJRPG_HeroProperties::CalculateNewStats()
{
	for (auto it = equippedWeapon.itemStats.CreateConstIterator(); it; ++it)
	{
		switch (it.Key())
		{
		case EItemStatType::HP:
			maxHealth = baseHealth + it.Value();
			break;
		case EItemStatType::MP:
			maxMana = baseMana + it.Value();
			break;
		case EItemStatType::PHYSICAL_ATTACK:
			currPhysicalAttack = physicalAttack + it.Value();
			break;
		case EItemStatType::PHYSICAL_DEFENSE:
			currPhysicalDefense = physicalDefense + it.Value();
			break;
		case EItemStatType::MAGIC_ATTACK:
			currMagicAttack = magicAttack + it.Value();
			break;
		case EItemStatType::MAGIC_DEFENSE:
			currMagicDefense = magicDefense + it.Value();
			break;
		}
	}

	for (auto it = equippedArmor.itemStats.CreateConstIterator(); it; ++it)
	{
		switch (it.Key())
		{
		case EItemStatType::HP:
			maxHealth = baseHealth + it.Value();
			break;
		case EItemStatType::MP:
			maxMana = baseMana + it.Value();
			break;
		case EItemStatType::PHYSICAL_ATTACK:
			currPhysicalAttack = physicalAttack + it.Value();
			break;
		case EItemStatType::PHYSICAL_DEFENSE:
			currPhysicalDefense = physicalDefense + it.Value();
			break;
		case EItemStatType::MAGIC_ATTACK:
			currMagicAttack = magicAttack + it.Value();
			break;
		case EItemStatType::MAGIC_DEFENSE:
			currMagicDefense = magicDefense + it.Value();
			break;
		}
	}

	for (auto it = equippedAmulet.itemStats.CreateConstIterator(); it; ++it)
	{
		switch (it.Key())
		{
		case EItemStatType::HP:
			maxHealth = baseHealth + it.Value();
			break;
		case EItemStatType::MP:
			maxMana = baseMana + it.Value();
			break;
		case EItemStatType::PHYSICAL_ATTACK:
			currPhysicalAttack = physicalAttack + it.Value();
			break;
		case EItemStatType::PHYSICAL_DEFENSE:
			currPhysicalDefense = physicalDefense + it.Value();
			break;
		case EItemStatType::MAGIC_ATTACK:
			currMagicAttack = magicAttack + it.Value();
			break;
		case EItemStatType::MAGIC_DEFENSE:
			currMagicDefense = magicDefense + it.Value();
			break;
		}
	}

	for (auto it = equippedRing.itemStats.CreateConstIterator(); it; ++it)
	{
		switch (it.Key())
		{
		case EItemStatType::HP:
			maxHealth = baseHealth + it.Value();
			break;
		case EItemStatType::MP:
			maxMana = baseMana + it.Value();
			break;
		case EItemStatType::PHYSICAL_ATTACK:
			currPhysicalAttack = physicalAttack + it.Value();
			break;
		case EItemStatType::PHYSICAL_DEFENSE:
			currPhysicalDefense = physicalDefense + it.Value();
			break;
		case EItemStatType::MAGIC_ATTACK:
			currMagicAttack = magicAttack + it.Value();
			break;
		case EItemStatType::MAGIC_DEFENSE:
			currMagicDefense = magicDefense + it.Value();
			break;
		}
	}
}