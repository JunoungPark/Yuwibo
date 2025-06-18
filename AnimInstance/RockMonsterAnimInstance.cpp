// Fill out your copyright notice in the Description page of Project Settings.


#include "RockMonsterAnimInstance.h"
#include "Yuwibo/Character/BaseCharacter.h"
#include "Yuwibo/Component/MoveComponent.h"
#include "Yuwibo/Component/AiComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
void URockMonsterAnimInstance::AnimNotify_StartAttack()
{
	if (!Character->HasAuthority()) return;

	TArray<FHitResult> HitResults;
	FCollisionQueryParams QueryParams(NAME_None, false, Character);
	QueryParams.AddIgnoredActor(Character);

	constexpr float AttackRange = 200.f;
	constexpr float CapsuleHalfHeight = 100.f;
	
	if (GetWorld()->SweepMultiByChannel(
		HitResults,
		Character->GetActorLocation(),
		Character->GetActorLocation() + Character->GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_Visibility,
		FCollisionShape::MakeCapsule(CapsuleHalfHeight, Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
		QueryParams))
	{
		TSet<ABaseCharacter*> Characters;

		for (FHitResult HitResult : HitResults)
		{
			if (HitResult.GetComponent()->IsA(UShapeComponent::StaticClass())) continue;

			auto HitCharacter = Cast<ABaseCharacter>(HitResult.GetActor());
			if (HitCharacter && HitCharacter != Character->Owner && !Characters.Contains(HitCharacter) )
			{
				Characters.Emplace(HitCharacter);
				UGameplayStatics::ApplyDamage(HitCharacter, 25.f, Character->GetInstigator()->Controller, Character, UDamageType::StaticClass());
				//HitCharacter->GetCharacterMovement()->AddImpulse(Character->GetActorForwardVector() * AttackRange * 100, true);
				//HitCharacter->GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector);
				//HitCharacter->GetMesh()->AddImpulseToAllBodiesBelow(Character->GetActorForwardVector() * AttackRange*2.5, NAME_None);

				auto LaunchVelocity = FRotator(0., Character->GetActorRotation().Yaw, 0.).Vector() * 500.f;

				LaunchVelocity.Z = 200.0f; // ��¦ ���� ƨ�ܼ� �ڿ�������

				HitCharacter->LaunchCharacter(LaunchVelocity, true, true);
			}
		}

	}
	DebugCapsule(AttackRange, CapsuleHalfHeight);
}

//void URockMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
//{
//	Super::NativeUpdateAnimation(DeltaSeconds);
//	if (IsValid(CharacterMovement))
//	{
//		if (Character->IsPlayerControlled()) return;
//
//		if(auto Move = Character->GetComponent<UMoveComponent>(Component::Move))
//		{
//			IsLook ? Move->SetCrouch<false>() : Move->SetCrouch<true>();
//
//
//			if (IsCrouch) IsAttack = false;
//		}
//	}
//}
