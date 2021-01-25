
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "JRPG_BattleAIController.generated.h"

UCLASS()
class JRPG_API AJRPG_BattleAIController : public AAIController
{
	GENERATED_BODY()
	
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

};
