// Fill out your copyright notice in the Description page of Project Settings.


#include "TankBackGunAnimInstance.h"
#include "Yuwibo/Character/TankLv3.h"
#include "Yuwibo/Component/HealthComponent.h"
#include "Yuwibo/Component/MoveComponent.h"
void UTankBackGunAnimInstance::AnimNotify_On()
{
	On.Broadcast(false);
}

void UTankBackGunAnimInstance::AnimNotify_Off()
{
	if(auto Pawn = TryGetPawnOwner())
	{
		if (!Pawn->HasAuthority()) return;

		if (IsValid(Pawn) && !Cast<ABaseCharacter>(Pawn)->GetComponent<UHealthComponent>(Component::Health)->IsDissolving())
		{
			if (auto Move = static_cast<ABaseCharacter*>(Pawn)->GetComponent<UMoveComponent>(Component::Move))
			{
				Move->SetCanMove(true);
				Move->SetCanAttack(true);
			}
		}
	}
}
