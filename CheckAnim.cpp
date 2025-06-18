// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckAnim.h"
#include "Component/AnimComponent.h"
#include "Character/GroundTurret.h"
#include "Character/TankLv3.h"
#include "Character/Soldier.h"
#include "Character/Vampire.h"
#include "Character/Moose.h"
#include "Character/Crustaspikan.h"
#include "Character/CrustaspikanLarvae.h"
#include "Character/RockMonster.h"
#include "Character/Trainer.h"
#include "Character/BodyChanger.h"
#include "Character/Drone.h"
UAnimComponent* CheckAnimHelper::CheckAnim(ABaseCharacter* Character)
{
	UAnimComponent* Anim = nullptr;

	if (Character->IsA(AGroundTurret::StaticClass()))
	{
		Anim = Character->CreateDefaultSubobject<UGroundTurretAnimComponent>("Anim");
	}
	if (Character->IsA(ABodyChanger::StaticClass()))
	{
		Anim = Character->CreateDefaultSubobject<UBodyChangerAnimComponent>("Anim");
	}
	if (Character->IsA(ADrone::StaticClass()))
	{
		Anim = Character->CreateDefaultSubobject<UDroneAnimComponent>("Anim");
	}
	else if (Character->IsA(ASoldier::StaticClass()))
	{
		Anim = Character->CreateDefaultSubobject<USoldierAnimComponent>("Anim");
	}
	else if (Character->IsA(ATankLv3::StaticClass()))
	{
		Anim = Character->CreateDefaultSubobject<UTankLv3AnimComponent>("Anim");
	}
	else if (Character->IsA(ATank::StaticClass()))
	{
		Anim = Character->CreateDefaultSubobject<UTankAnimComponent>("Anim");
	}
	else if (Character->IsA(ATrainer::StaticClass()))
	{
		Anim = Character->CreateDefaultSubobject<UTrainerAnimComponent>("Anim");
	}
	else if (Character->IsA(AMoose::StaticClass()))
	{
		Anim = Character->CreateDefaultSubobject<UMooseAnimComponent>("Anim");
	}
	else if (Character->IsA(ARockMonster::StaticClass()))
	{
		Anim = Character->CreateDefaultSubobject<URockMonsterAnimComponent>("Anim");
	}
	else if (Character->IsA(ACrustaspikan::StaticClass()) || Character->IsA(ACrustaspikanLarvae::StaticClass()) || Character->IsA(AVampire::StaticClass()))
	{
		Anim = Character->CreateDefaultSubobject<UEnemyAnimComponent>("Anim");
	}

	return Anim;
}
