// Fill out your copyright notice in the Description page of Project Settings.


#include "JRPG_BaseAbility.h"

void UJRPG_BaseAbility::UpgradeAbility()
{
	abilityLevel++;
	abilityDamage *= 1.2;
}
