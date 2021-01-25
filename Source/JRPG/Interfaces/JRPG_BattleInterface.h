#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include <JRPG\Data\JRPG_BaseAbility.h>
#include "JRPG_BattleInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UJRPG_BattleInterface : public UInterface
{
	GENERATED_BODY()
};


class JRPG_API IJRPG_BattleInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BattleInterface")
		void PerformAction();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BattleInterface")
		void MoveComplete();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BattleInterface")
		void SetShouldTick(bool bTick);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BattleInterface")
		FVector GetAttackPos();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BattleInterface")
		void HealthChanged(float healthAmount, EMagicType magicType, EElementalType attackerType);
};
