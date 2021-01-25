#include "JRPG_BaseHero.h"
#include "../Data/JRPG_HeroProperties.h"
#include "../Data/JRPG_BaseAbility.h"
#include "JRPG_BattleAIController.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "../Data/HelperFunctions.h"
#include "JRPG_BaseEnemy.h"

AJRPG_BaseHero::AJRPG_BaseHero()
{
	weaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	if (weaponMesh)
	{
		weaponMesh->SetupAttachment(GetMesh(),FName(TEXT("Weapon_Socket")));
		
		weaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("Hero");
	
	//AIControllerClass = AJRPG_BattleAIController::StaticClass();
}


void AJRPG_BaseHero::BeginPlay()
{
	Super::BeginPlay();

	//setup battle positions
	startPos = GetActorLocation();
	attackPos = startPos + (GetActorForwardVector() * 150.f);
	startRotation = GetActorRotation();
	rangeAttackPos = startPos + GetActorForwardVector() * 200;
}

void AJRPG_BaseHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldTick)
	{
		switch (heroState)
		{
		case EHeroState::PROCESSING:
			currentCooldown += DeltaTime;
			if (currentCooldown >= maxCooldown)
			{
				heroState = EHeroState::CHOOSE_ACTION;
			}
			break;
		case EHeroState::CHOOSE_ACTION:
			//set herobutton enabled
			bButtonEnabled = true;
			heroState = EHeroState::INPUT;
			break;
		case EHeroState::INPUT:
			//idle state to wait until input is done
			break;
		case EHeroState::WAITING:
			//idle state to wait for turn
			break;
		case EHeroState::PERFORM_ACTION:

			break;
		case EHeroState::DEAD:
			Die();
			bShouldTick = false;
			break;
		default:
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("ERROR OCCURED!! BaseHero failed to switch on battlestate!!")));
			break;
		}
	}
}

void AJRPG_BaseHero::SpawnWeapon()
{
	weaponMesh->SetStaticMesh(HeroProperties->equippedWeapon.itemMesh);
}

void AJRPG_BaseHero::SetAttack(AActor* Target, EActionType actionType, UJRPG_BaseAbility* ability, FItemDynamicData item)
{
	bButtonEnabled = false;

	myAttack.target = Target;
	myAttack.turnType = actionType;
	myAttack.Ability = ability;
	myAttack.item = item;
	//setup battle position based on attacktype
	switch (actionType)
	{
	case EActionType::ATTACK: //meleeattack

		if (myAttack.target->GetClass()->ImplementsInterface(UJRPG_BattleInterface::StaticClass()))
		{
			turnPos = IJRPG_BattleInterface::Execute_GetAttackPos(myAttack.target);
		}

		if (AttackAnimMontage)
			AnimMontageToPlay = AttackAnimMontage;
		
		break;
	case EActionType::ABILITY:
		if (ability)
		{   // melee ability
			if (ability->abilityRange == EAbilityRange::MELEE)
			{
				if (myAttack.target->GetClass()->ImplementsInterface(UJRPG_BattleInterface::StaticClass()))
				{
					turnPos = IJRPG_BattleInterface::Execute_GetAttackPos(myAttack.target);
				}
				if (MeleAbilityAnimMontage)
					AnimMontageToPlay = MeleAbilityAnimMontage;

				if(ability->weaponEffects)
				{ 
					UGameplayStatics::SpawnEmitterAttached(ability->weaponEffects, Cast<USceneComponent>(FindComponentByClass(UStaticMeshComponent::StaticClass())), FName(TEXT("FX_Socket")), FVector(0,0,0), FRotator(0,0,0), EAttachLocation::SnapToTarget, false, EPSCPoolMethod::AutoRelease,true);
				}
			}
			else//range combat ability
			{
				turnPos = rangeAttackPos;
				if (MagicAnimMontage)
					AnimMontageToPlay = MagicAnimMontage;
			}
		}
		break;
	case EActionType::ITEM: //items are used at range attack position
		turnPos = rangeAttackPos;
		if (MagicAnimMontage)
			AnimMontageToPlay = MagicAnimMontage;
		break;
	default:
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("ERROR OCCURED!! BaseHero failed to switch on actionType!!")));
		break;
	}
	
	BGM->AttackerList.Add(this);
	currentCooldown = 0;
	bStartAction = true;
	heroState = EHeroState::WAITING;
}

void AJRPG_BaseHero::SetShouldTick_Implementation(bool bTick)
{
	bShouldTick = bTick;
}

void AJRPG_BaseHero::PerformAction_Implementation()
{
	if (bStartAction == true)
	{
		bStartAction = false;
		MoveToPosition(turnPos);
		FString ht = (HeroProperties->heroName + FString(TEXT(" is attacking")));
		BGM->SetHelpText(ht);
	}
}

void AJRPG_BaseHero::MoveToPosition(FVector position)
{
	AJRPG_BattleAIController* BattleController = Cast<AJRPG_BattleAIController>(GetController());
	if(BattleController)
	BattleController->MoveToLocation(position, 50.0f, false);
}

void AJRPG_BaseHero::MoveComplete_Implementation()
{
	//if attack.target is valid we want to attack the target and reset turninfo and then move backt to start
	if (myAttack.target)
	{
		float delay = PlayAnimations();
		//apply damage

		if (myAttack.turnType == EActionType::ITEM)
		{
			AJRPG_BaseHero* targetHero = Cast<AJRPG_BaseHero>(myAttack.target);
			UHelperFunctions::UseItem(Cast<UJRPG_GameInstance>(GetGameInstance()), myAttack.item, targetHero->HeroProperties);
		}
		else
		{
			if (myAttack.target->GetClass()->ImplementsInterface(UJRPG_BattleInterface::StaticClass()))
			{
				EMagicType mType = EMagicType::NONE;
				EElementalType eType = EElementalType::NONE;
				if (myAttack.Ability)
				{
					mType = myAttack.Ability->abilityType;
					eType = myAttack.Ability->abilityElementalType;
				}
				IJRPG_BattleInterface::Execute_HealthChanged(myAttack.target, CalculateDamage(), mType, eType);
			}
		}
		//reset myAttack
		myAttack.target = nullptr;
		myAttack.Ability = nullptr;
		myAttack.item;

		FTimerDelegate TimerDel;
		FTimerHandle TimerHandle;

		//Binding the function with specific values
		TimerDel.BindUFunction(this, FName("MoveToPosition"), startPos);
		//Calling Move Function after animation without looping
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, delay, false);
	}
	//if attack.target is not valid it means attack is done and we can complete our turn
	else
	{
		SetActorRotation(startRotation);

		for (USceneComponent* comp : weaponMesh->GetAttachChildren())
		{
			comp->DestroyComponent();
		}

		heroState = EHeroState::PROCESSING;
		BGM->currentBattleState = EBattleState::CHECK_ALIVE;
	}
}

FVector AJRPG_BaseHero::GetAttackPos_Implementation()
{
	return attackPos;
}

void AJRPG_BaseHero::HealthChanged_Implementation(float healthAmount, EMagicType magicType, EElementalType attackerType)
{
	float calculatedHealthAmount = 0;
	//physical attack
	if (magicType == EMagicType::NONE)
	{
		calculatedHealthAmount = FMath::Clamp((healthAmount - HeroProperties->currPhysicalDefense), 0.0f, HeroProperties->maxHealth);
	}
	//magic attack
	if (magicType == EMagicType::BLACK_MAGIC)
	{
		calculatedHealthAmount = FMath::Clamp((healthAmount - HeroProperties->currMagicDefense), 0.0f, HeroProperties->maxHealth);
	}
	//heal
	if (magicType == EMagicType::WHITE_MAGIC)
	{
		calculatedHealthAmount = healthAmount;
	}
	HeroProperties->TakeDamage(calculatedHealthAmount);

	if (HeroProperties->currentHealth <= 0)
	{
		heroState = EHeroState::DEAD;
	}
}

float AJRPG_BaseHero::PlayAnimations()
{
	float animDuration = 1;


	animDuration = PlayAnimMontage(AnimMontageToPlay, 1.0f);

	if (myAttack.Ability)
	{
		if (myAttack.Ability->abilityEffects)
		{
			UGameplayStatics::SpawnEmitterAttached(myAttack.Ability->abilityEffects, Cast<USceneComponent>(myAttack.target->FindComponentByClass(USkeletalMeshComponent::StaticClass())), NAME_None, FVector(0, 0, 64), FRotator(0, 0, 0), FVector(1,1,1), EAttachLocation::SnapToTargetIncludingScale, true, EPSCPoolMethod::AutoRelease, true);
		}
	}

	return animDuration;
}

float AJRPG_BaseHero::CalculateDamage()
{
	float calcDamage = 0;
	switch (myAttack.turnType)
	{
		case EActionType::ATTACK:
			calcDamage = HeroProperties->currPhysicalAttack;
			break;
		case EActionType::ABILITY:
			if(myAttack.Ability->abilityType == EMagicType::BLACK_MAGIC)
				calcDamage = myAttack.Ability->abilityDamage + HeroProperties->currMagicAttack;
			else if(myAttack.Ability->abilityType == EMagicType::WHITE_MAGIC)
				calcDamage = myAttack.Ability->abilityDamage - HeroProperties->currMagicAttack;


			HeroProperties->currentMana -= myAttack.Ability->abilityManaCost;

			break;
		case EActionType::ITEM:

			//calcDamage = item.damage;
			break;
	default:
		break;
	}
	return calcDamage;
}

void AJRPG_BaseHero::Die()
{
	bButtonEnabled = false;
	bShouldTick = false;
	heroState = EHeroState::WAITING;
	BGM->HeroesInBattle.Remove(this);
	if (BGM->AttackerList.Contains(this))
	{
		BGM->AttackerList.Remove(this);
	}
	//check if this is an enemy target
	for (int i = 0; i < BGM->AttackerList.Num(); i++)
	{
		AJRPG_BaseEnemy* enemy = Cast<AJRPG_BaseEnemy>(BGM->AttackerList[i]);
		if (enemy)
		{
			if (enemy->myAttack.target == this)
			{
				if (BGM->HeroesInBattle.Num() > 0)
				{
					enemy->myAttack.target = BGM->HeroesInBattle[FMath::RandRange(0, BGM->HeroesInBattle.Num() - 1)];
				}
			}
		}
	}

	bDead = true;

}