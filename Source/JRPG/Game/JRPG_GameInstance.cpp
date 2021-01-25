#include "JRPG_GameInstance.h"
#include "../Data/JRPG_HeroProperties.h"

void UJRPG_GameInstance::Init()
{
	Super::Init();

	//set up hero team
	for (int i = 0; i < DefaultHeroPartyClasses.Num(); i++)
	{
		UJRPG_HeroProperties* hero = NewObject<UJRPG_HeroProperties>(this, DefaultHeroPartyClasses[i]->GetFName(), RF_NoFlags, DefaultHeroPartyClasses[i].GetDefaultObject());
		if (hero)
		{
			hero->Initialize();
			HeroParty.Insert(hero, i);
		}
	}
}