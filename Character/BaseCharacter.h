// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Blueprint/UserWidget.h"
#include "Yuwibo/Animinstance/BaseAnimInstance.h"
#include "GenericTeamAgentInterface.h"
#include "Perception/AISense_Hearing.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Yuwibo/CheckAnim.h"
#include "BaseCharacter.generated.h"

UENUM()
enum class Component : uint8
{
	Information,
	Anim,
	Map,
	Health,
	Look,
	Move,
	FootIk,
	Equipment,
	AI,
	Enemy,
	AIControll,
	Item,
	Skill
};

UCLASS()
class YUWIBO_API ABaseCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
private:
	FGenericTeamId TeamID;
protected:
	FORCEINLINE void SetTeamId(const FGenericTeamId& NewTeamID) { TeamID = NewTeamID; };
public:
	FORCEINLINE FGenericTeamId GetGenericTeamId() const override { return TeamID; }
public:
	// Sets default values for this character's properties
	ABaseCharacter();
private:


	FRotator SocketRotation;


	UPROPERTY()
	TMap<Component, UActorComponent*> Components;

	UPROPERTY()
	UUserWidget* Alarm;

	FTimerHandle AlarmTimerHandle;
private:

	FORCEINLINE void CollapsedAlarm() { Alarm->SetVisibility(ESlateVisibility::Collapsed); }
protected:
	// Called when the game starts or when spawned
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName CharacterName;

protected:
	template<typename T>
	FORCEINLINE T* SetComponent(Component type)
	{
		FName name;

		T* NewComponent = nullptr;

		switch (type)
		{
		case Component::Information:
			name = "Information";
			break;
		case Component::Anim:
			name = "Anim";
			NewComponent = Cast<T>(CheckAnimHelper::CheckAnim(this));
			break;
		case Component::Map:
			name = "Map";
			break;
		case Component::Health:
			name = "Health";
			break;
		case Component::Look:
			name = "Look";
			break;
		case Component::Move:
			name = "Move";
			break;
		case Component::FootIk:
			name = "FootIk";
			break;
		case Component::Equipment:
			name = "Equipment";
			break;
		case Component::AI:
			name = "AI";
			break;
		case Component::Enemy:
			name = "Enemy";
			break;
		case Component::AIControll:
			name = "AIControll";
			break;
		case Component::Item:
			name = "Item";
			break;
		case Component::Skill:
			name = "Skill";
			break;
		default:
			break;
		}
		if (!NewComponent) NewComponent = CreateDefaultSubobject<T>(name);

		NewComponent->Bind(this);

		Components.Emplace(type, NewComponent);

		return NewComponent;
	}
	 
public:	

	virtual void GetActorEyesViewPoint(FVector& out_Location, FRotator& out_Rotation) const override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Health")
	float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure)
	FORCEINLINE FName GetCharacterName() const { return CharacterName; }
		
		template<typename T>
		FORCEINLINE T* GetComponent(Component type)
		{
			if (UActorComponent** Comp = Components.Find(type))
				return static_cast<T*>(*Comp);
			else
				return nullptr;
		}

		FORCEINLINE bool GetIsAttack() const { return static_cast<UBaseAnimInstance*>(GetMesh()->GetAnimInstance())->GetIsAttack(); }

		FORCEINLINE void Noise(FName tag = NAME_None)
		{
			UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), 1.f, this, 1000.f, tag);
		}

		void Tick(float DeltaTime)override;

		UFUNCTION(Client, Reliable)
		void VIsibleAlarm();

};