#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "JRPG_InteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UJRPG_InteractInterface : public UInterface
{
	GENERATED_BODY()
};


class JRPG_API IJRPG_InteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
		void Interact(AActor* Caller);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
		void StartFocus();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
		void EndFocus();
};
