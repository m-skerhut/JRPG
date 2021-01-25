#pragma once

#include "CoreMinimal.h"
#include <JRPG/Game/Interactables/JRPG_BaseInteractable.h>
#include "JRPG_Shop.generated.h"

UCLASS()
class JRPG_API AJRPG_Shop : public AJRPG_BaseInteractable
{
	GENERATED_BODY()

public:
	AJRPG_Shop();

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ShopWidgetClass;
	UPROPERTY()
	class UUserWidget* ShopWidget;

	void Interact(AActor* Caller);//prototype declaration
	virtual void Interact_Implementation(AActor* Caller) override; //actual implementation of interact method

protected:

	UFUNCTION(BlueprintCallable)
	void ToggleShopWidget();

	//sell items to player
	UFUNCTION(BlueprintCallable)
	void SellItems();
	//buy items from player
	UFUNCTION(BlueprintCallable)
	void BuyItems();
};
