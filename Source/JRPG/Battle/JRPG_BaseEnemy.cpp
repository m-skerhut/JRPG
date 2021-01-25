#include "JRPG_BaseEnemy.h"
#include "JRPG_BattleAIController.h"
#include "TimerManager.h"
#include <JRPG/Data/JRPG_BaseAbility.h>
#include "Blueprint/UserWidget.h"

AJRPG_BaseEnemy::AJRPG_BaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	/*SelectorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SelectorMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	SelectorMesh->SetSimulatePhysics(false);
	SelectorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);*/
	Tags.Add("Enemy");
}


void AJRPG_BaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	startPos = GetActorLocation();
	attackPos = startPos + (GetActorForwardVector() * 150.f);
	startRotation = GetActorRotation();
	myAttack.attacker = this;
	enemyState = EEnemySate::PROCESSING;
	//SelectorMesh->SetVisibility(false);
}


void AJRPG_BaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldTick)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("%s health: %f"), *enemyName, currentHealth));

		switch (enemyState)
		{
		case EEnemySate::PROCESSING:
			currentCooldown += DeltaTime;
			if (currentCooldown >= maxCooldown)
			{
				enemyState = EEnemySate::CHOOSE_ACTION;
			}
			break;
		case EEnemySate::CHOOSE_ACTION:

			ChooseAttack();
			break;
		case EEnemySate::WAITING:
			//idle state to wait for turn
			break;
		case EEnemySate::PERFORM_ACTION:


			break;
		case EEnemySate::DEAD:
			Die();
			bShouldTick = false;
			break;
		default:
			break;
		}
	}
}

void AJRPG_BaseEnemy::ChooseAttack()
{
	//myAttack.attackType = EActionRange::MELEE;
	if (BGM->HeroesInBattle.Num() > 0)
	{
		myAttack.target = BGM->HeroesInBattle[FMath::RandRange(0, BGM->HeroesInBattle.Num() - 1)];

		if (myAttack.target->GetClass()->ImplementsInterface(UJRPG_BattleInterface::StaticClass()))
		{
			turnPos = IJRPG_BattleInterface::Execute_GetAttackPos(myAttack.target);
		}
	}



	BGM->AttackerList.Add(this);
	currentCooldown = 0;
	bStartAction = true;
	enemyState = EEnemySate::WAITING;
}

void AJRPG_BaseEnemy::PerformAction_Implementation()
{
	if (bStartAction == true)
	{
		bStartAction = false;
		if (myAttack.target)
		{
			MoveToPosition(turnPos);
			BGM->SetHelpText(enemyName + (TEXT(" is attacking.")));
		}
	}
}

void AJRPG_BaseEnemy::MoveToPosition(FVector position)
{
	AJRPG_BattleAIController* BattleController = Cast<AJRPG_BattleAIController>(GetController());
	BattleController->MoveToLocation(position, 50.0f, false);
}

void AJRPG_BaseEnemy::MoveComplete_Implementation()
{
	//if attack.target is valid we want to attack the target and reset turninfo and then move backt to start
	if (myAttack.target)
	{
		float animDuration = 1;
		if (AttackAnimMontage)
		{
			animDuration = PlayAnimMontage(AttackAnimMontage, 1.0f);
		}
		
		if (myAttack.target->GetClass()->ImplementsInterface(UJRPG_BattleInterface::StaticClass()))
		{
			IJRPG_BattleInterface::Execute_HealthChanged(myAttack.target, CalculateDamage(), EMagicType::NONE, EElementalType::NONE);
		}
		
		
		myAttack.target = nullptr;
		myAttack.Ability = nullptr;

		FTimerDelegate TimerDel;
		FTimerHandle TimerHandle;

		//Binding the function with specific values
		TimerDel.BindUFunction(this, FName("MoveToPosition"), startPos);
		//Calling MoveFunction after animation without looping
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, animDuration, false);
	}
	//if attack.target is not valid it means attack is done and we can complete our turn
	else
	{
		SetActorRotation(startRotation);

		enemyState = EEnemySate::PROCESSING;
		BGM->currentBattleState = EBattleState::CHECK_ALIVE;
	}
}

void AJRPG_BaseEnemy::SetShouldTick_Implementation(bool bTick)
{
	bShouldTick = bTick;
}

FVector AJRPG_BaseEnemy::GetAttackPos_Implementation()
{
	return attackPos;
}


void AJRPG_BaseEnemy::HealthChanged_Implementation(float healthAmount, EMagicType magicType, EElementalType attackerType)
{
	//calculate damage based on effectiveness and defense
	float calculatedHealthAmount = 0;

	//physical attack
	if (magicType == EMagicType::NONE)
	{
		calculatedHealthAmount = FMath::Clamp((healthAmount - physicalDefense), 0.0f, maxHealth);
	}
	//magic attack
	if (magicType == EMagicType::BLACK_MAGIC)
	{
		calculatedHealthAmount = FMath::Clamp(((healthAmount * TypeChart::GetEffectiveness(attackerType, elementalType)) - magicDefense) , 0.0f, maxHealth);
	}
	//
	if (magicType == EMagicType::WHITE_MAGIC)
	{
		calculatedHealthAmount = healthAmount;
	}

	currentHealth -= calculatedHealthAmount;
	if (currentHealth <= 0)
	{
		enemyState = EEnemySate::DEAD;
	}
}

float AJRPG_BaseEnemy::CalculateDamage()
{
	return physicalAttack;
}

void AJRPG_BaseEnemy::Die()
{
	enemyState = EEnemySate::WAITING;
	BGM->EnemiesInBattle.Remove(this);
	targetButton->RemoveFromParent();
	if (BGM->AttackerList.Contains(this))
	{
		BGM->AttackerList.Remove(this);
	}
	BGM->DeadEnemies.Add(this);
	PlayDissolveEffect();
	//this->SetActorHiddenInGame(true);
}

void AJRPG_BaseEnemy::PlayDissolveEffect_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("playing dissolveeffect")));
}
