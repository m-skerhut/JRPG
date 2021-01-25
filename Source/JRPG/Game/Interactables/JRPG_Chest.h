#pragma once

#include "CoreMinimal.h"
#include <JRPG/Game/Interactables/JRPG_BaseInteractable.h>
#include <JRPG/Game/JRPG_GameInstance.h>
#include "JRPG_Chest.generated.h"

UCLASS()
class JRPG_API AJRPG_Chest : public AJRPG_BaseInteractable
{
	GENERATED_BODY()
	
public:
	AJRPG_Chest();

	//UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interact")
	void Interact(AActor* Caller);//prototype declaration
	virtual void Interact_Implementation(AActor* Caller) override; //actual implementation of interact method

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	TArray<FItemDynamicData> items;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	int gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	class UStaticMesh* OpenChestMesh;

	//class UObtainedItemWidget* ;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UObtainedItemWidget> obtainedWidgetClass;
	UPROPERTY()
		class UObtainedItemWidget* itemsWidget;

protected:

	virtual void BeginPlay() override;

private:
	bool bOpen;
};
