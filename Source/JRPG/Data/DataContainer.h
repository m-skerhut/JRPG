#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DataContainer.generated.h"

UENUM(BlueprintType)
enum class EElementalType : uint8
{
	NONE UMETA(DisplayName = "none"),
	WATER UMETA(DisplayName = "Water"),
	FIRE UMETA(DisplayName = "Fire"),
	ELECTRO UMETA(DisplayName = "Electro"),
	GRASS UMETA(DisplayName = "Grass")
};



UENUM(BlueprintType)
enum class ERarity : uint8
{
	COMMON UMETA(DisplayName = "Common"),
	RARE UMETA(DisplayName = "Rare"),
	EPIC UMETA(DisplayName = "Epic"),
	LEGENDARY UMETA(DisplayName = "Legendary")
};




#pragma region Items


UENUM(BlueprintType)
enum class EItemType : uint8
{
	NA,
	CONSUMABLE,
	WEAPON,
	ARMOR,
	AMULET,
	RING,
	QUESTITEM
};

UENUM(BlueprintType)
enum class EItemStatType : uint8
{
	NA,
	HP,
	MP,
	PHYSICAL_ATTACK,
	PHYSICAL_DEFENSE,
	MAGIC_ATTACK,
	MAGIC_DEFENSE
};

//base struct for every item
USTRUCT(BlueprintType)
struct FBaseItemProperties : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Properties")
	FName itemRowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Properties")
	FString itemDisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Properties")
	EItemType itemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Properties")
	ERarity itemRarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Properties")
	FString itemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Properties")
	TMap<EItemStatType, float> itemStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Properties")
	FString useButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Properties")
	uint8 value = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Properties")
	class UTexture2D* itemThumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Properties")
	class UStaticMesh* itemMesh;
};



USTRUCT(BlueprintType)
struct FItemDynamicData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 amount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName dtRowName;

};

#pragma endregion


class JRPG_API DataContainer
{
public:

	DataContainer();
	~DataContainer();
};


static class JRPG_API TypeChart
{
	
private:

	static constexpr float chart[4][4] =
	{
		//			   water fire  elec  grass
		/*water*/     {0.5f, 2.0f, 0.5f, 1.0f},
		/*fire*/      {0.5f, 0.5f, 1.0f, 2.0f},
		/*electro*/   {2.0f, 1.0f, 0.5f, 0.5f},
		/*grass*/     {1.0f, 0.5f, 2.0f, 0.5f}
	};

public:

	static float GetEffectiveness(EElementalType attackerType, EElementalType defenderType)
	{
		if (attackerType == EElementalType::NONE || defenderType == EElementalType::NONE)
		{
			return 1;
		}

		int row = (int)attackerType - 1;
		int col = (int)defenderType - 1;

		return chart[row][col];
	};
};