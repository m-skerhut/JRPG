#include "JRPG_Shop.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"

AJRPG_Shop::AJRPG_Shop()
{

}

void AJRPG_Shop::Interact_Implementation(AActor* Caller)
{
	ToggleShopWidget();
}

void AJRPG_Shop::ToggleShopWidget()
{
	APlayerController* PController = GetWorld()->GetFirstPlayerController();
	if (ShopWidget)
	{
		//remove from viewport
		ShopWidget->RemoveFromParent();
		PController->SetInputMode(FInputModeGameOnly());
		PController->SetPause(false);
		PController->bShowMouseCursor = false;
		ShopWidget = nullptr;
	}
	else
	{
		//add toviewport
		ShopWidget = CreateWidget<UUserWidget>(GetWorld(), ShopWidgetClass);
		ShopWidget->AddToViewport();
		PController->SetInputMode(FInputModeGameAndUI());
		PController->SetPause(true);
		PController->bShowMouseCursor = true;
	}
}

void AJRPG_Shop::SellItems()
{

}

void AJRPG_Shop::BuyItems()
{

}