#include "HelperFunctions.h"
#include "Engine.h"

void UHelperFunctions::AddItemToInventory(UJRPG_GameInstance* gameInstance, FItemDynamicData item)
{
	bool bContainsItem = false;
	int index = -1;

	if (gameInstance)
	{
		//check if item is already in inventory
		for (int i = 0; i < gameInstance->inventory.Num(); i++)
		{
			if (item.dtRowName == gameInstance->inventory[i].dtRowName)
			{
				bContainsItem = true;
				index = i;
				break;
			}
		}

		if (bContainsItem)
		{
			gameInstance->inventory[index].amount += item.amount;
		}
		else
		{
			gameInstance->inventory.Add(item);
		}
	}
}

void UHelperFunctions::UseItem(UJRPG_GameInstance* gameInstance, FItemDynamicData item, UJRPG_HeroProperties* hero)
{
	static const FString contextString(TEXT("ItemContextString"));
	FBaseItemProperties* itemProps = gameInstance->itemDataTable->FindRow<FBaseItemProperties>(item.dtRowName, contextString, true);
	if (itemProps)
	{
		switch (itemProps->itemType)
		{
		case EItemType::CONSUMABLE:

			for (auto It = itemProps->itemStats.CreateConstIterator(); It; ++It)
			{
				switch (It.Key())
				{
				case EItemStatType::HP:
					hero->RestoreHealth(It.Value());
					break;
				case EItemStatType::MP:
					hero->RestoreMana(It.Value());
					break;
				default:
					break;
				}
			}
			break;
		case EItemType::WEAPON:

		case EItemType::ARMOR:

		case EItemType::AMULET:

		case EItemType::RING:

			hero->ChangeEquippment(itemProps);
			break;
		default:
			break;
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("rowname %s"),*item.dtRowName.ToString()));
	}
	RemoveItem(gameInstance, item);
}

void UHelperFunctions::RemoveItem(UJRPG_GameInstance* gameInstance, FItemDynamicData item)
{
	for (int i = 0; i < gameInstance->inventory.Num(); i++)
	{
		if (item.dtRowName == gameInstance->inventory[i].dtRowName)
		{
			if (item.amount > 1)
			{
				gameInstance->inventory[i].amount -= 1;
			}
			else
			{
				gameInstance->inventory.RemoveAt(i);
			}
			break;
		}
	}
}

TArray<FItemDynamicData> UHelperFunctions::GetAllItemsOfType(UJRPG_GameInstance* gameInstance, EItemType itemType)
{
	TArray<FItemDynamicData> items;

	static const FString contextString(TEXT("ItemContextString"));

	for (FItemDynamicData itemData : gameInstance->inventory)
	{
		FBaseItemProperties* itemProps = gameInstance->itemDataTable->FindRow<FBaseItemProperties>(itemData.dtRowName, contextString, true);
		if (itemProps->itemType == itemType)
		{
			items.Add(itemData);
		}
	}

	return items;
}

UJRPG_HeroProperties* UHelperFunctions::CalculateNewHeroStats(UJRPG_HeroProperties* hero, FBaseItemProperties item)
{
	auto* newHeroStats = NewObject<UJRPG_HeroProperties>(hero->GetWorld(), TEXT("CopyTexture"), RF_Transient, hero);
	newHeroStats->ChangeEquippment(&item);
	return newHeroStats;
}

