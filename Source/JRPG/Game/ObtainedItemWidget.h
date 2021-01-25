#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Data/DataContainer.h"
#include "ObtainedItemWidget.generated.h"


UCLASS()
class JRPG_API UObtainedItemWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    virtual void NativeConstruct() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parent Widgets")
    TArray<FItemDynamicData> obtainedItems;
};
