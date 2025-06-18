// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveCharacter.h"
#include "Yuwibo/Component/MoveComponent.h"
#include "Yuwibo/Component/ItemComponent.h"
#include "Components/AudioComponent.h"
AMoveCharacter::AMoveCharacter()
{
	SetComponent<UMoveComponent>(Component::Move);

	SetComponent<UItemComponent>(Component::Item);

	FootAudio = CreateDefaultSubobject<UAudioComponent>("FootAudio");

	FootAudio->SetAutoActivate(false);
}

void AMoveCharacter::IsClient(bool IsTrue)
{
	if (!IsLocallyControlled()) return;

	LocallyControll(IsTrue);

}

void AMoveCharacter::LocallyControll_Implementation(bool IsTrue)
{
	if (auto Move = GetComponent<UMoveComponent>(Component::Move))
	{
		IsTrue ? Move->SetAttack<true>() : Move->SetAttack<false>();
	}
}

void AMoveCharacter::IsServer_Implementation(bool IsTrue)
{
	if (HasAuthority() || IsLocallyControlled()) return;
	
	if (auto Move = GetComponent<UMoveComponent>(Component::Move))
	{
		IsTrue ? Move->SetAttack<true>() : Move->SetAttack<false>();
	}
}

void AMoveCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(auto Move = GetComponent<UMoveComponent>(Component::Move))
	{
		Move->OnAttack.AddUFunction(GetMesh()->GetAnimInstance(), FName("PlayAttackAnimation"));

		if (auto Anim = GetComponent<UAnimComponent>(Component::Anim))
		{
			Move->OnOffAttack.AddUFunction(Anim, FName("SetIsAttack"));

			Move->OnOffCrouch.BindUFunction(Anim, FName("SetIsCrouch"));
		}

		HasAuthority() ? Move->OnOffAttack.AddUFunction(this, FName("IsServer")) : Move->OnOffAttack.AddUFunction(this, FName("IsClient"));
	}
}

void AMoveCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto Item = GetComponent<UItemComponent>(Component::Item);

	PlayerInputComponent->BindAction(TEXT("OpenItemBox"), EInputEvent::IE_Pressed, Item, &UItemComponent::OpenItemHolder);

	UMoveComponent* Move = GetComponent<UMoveComponent>(Component::Move);

	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, Move, &UMoveComponent::SetAttack<true>);
	PlayerInputComponent->BindAxis(TEXT("MoveUpDown"), Move, &UMoveComponent::MoveUpDown);
	PlayerInputComponent->BindAxis(TEXT("MoveLeftRight"), Move, &UMoveComponent::MoveLeftRight);
}

void AMoveCharacter::PlayFootSound()
{
	if (!FootAudio->IsActive())
	{
		FootAudio->Activate();
	}
	else
	{
		FootAudio->Play();
	}
}