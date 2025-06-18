// Fill out your copyright notice in the Description page of Project Settings.


#include "CrustaspikanLarvaeAnimInstance.h"
#include "Yuwibo/Character/CrustaspikanLarvae.h"
#include "Yuwibo/Component/AiComponent.h"

void UCrustaspikanLarvaeAnimInstance::AnimNotify_EndMontageMove()
{
	Super::AnimNotify_EndMontageMove();
	Character->GetComponent<UEnemyComponent>(Component::Enemy)->EmptyAttackCharacters();
}
