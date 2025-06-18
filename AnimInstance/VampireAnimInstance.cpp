// Fill out your copyright notice in the Description page of Project Settings.


#include "VampireAnimInstance.h"
#include "Yuwibo/Character/Vampire.h"
#include "Yuwibo/Component/AiComponent.h"
void UVampireAnimInstance::AnimNotify_EndMontageMove()
{
	Super::AnimNotify_EndMontageMove();
	Character->GetComponent<UEnemyComponent>(Component::Enemy)->EmptyAttackCharacters();

}
