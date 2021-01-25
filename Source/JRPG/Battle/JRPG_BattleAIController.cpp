#include "JRPG_BattleAIController.h"
#include "../Interfaces/JRPG_BattleInterface.h"



void AJRPG_BattleAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	StopMovement();
	//if (AttackerList[0]->GetClass()->ImplementsInterface(UJRPG_BattleInterface::StaticClass()))
	//{
	//	IJRPG_BattleInterface::Execute_MoveComplete(AttackerList[0]);
	//}
	//get the controled actor and call move complete
	AActor* controlledPawn = GetInstigator();
	if (controlledPawn->GetClass()->ImplementsInterface(UJRPG_BattleInterface::StaticClass()))
	{
		IJRPG_BattleInterface::Execute_MoveComplete(controlledPawn);
	}
}
