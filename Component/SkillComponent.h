// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ActorComponent.h"
#include "SkillComponent.generated.h"
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class YUWIBO_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()
private:

	double CoolTime = 2.;

	double SecondCoolTime = 180.;

	UPROPERTY()
	FTimerHandle CooldownTimerHandle[2];

	UPROPERTY(ReplicatedUsing = OnRep_CanSkill)
	bool CanSkill = true;

	UPROPERTY(ReplicatedUsing = OnRep_CanSecondSkill)
	bool CanSecondSkill = true;

	UPROPERTY()
	class USkillWidget* Skill;

	class ABaseCharacter* Character;
public:
	// Sets default values for this component's properties
	USkillComponent();

private:
	void UpdateSkillUI(class UImage* Image, class UTextBlock* TextBlock, bool bCanUse, float InCoolTime, uint8 HandleIndex);

	void TimeCount(UTextBlock* TextBlock);

	void CanSkillTimeCount();

	void CanSecondSkillTimeCount();

	FORCEINLINE void SetCanSkill() { CanSkill = true; }

	FORCEINLINE void SetCanSecondSkill() { CanSecondSkill = true; }

protected:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	FORCEINLINE void SetCoolTime(double Time) { CoolTime = Time; };

	FORCEINLINE void SetSecondCoolTime(double Time) { SecondCoolTime = Time; };

	UFUNCTION()
	void OnRep_CanSkill();

	UFUNCTION()
	void OnRep_CanSecondSkill();

	void DestroyWidget();

	void AddViewportWidget();

	UFUNCTION(Server, Reliable)
	void GetCoolTime();

	UFUNCTION(Client, Reliable)
	void TimerStart(double RemainingTime, double SecondRemainingTime);

	void StartTimer(UImage* Image, UTextBlock* TextBlock, bool bCanUse, float InCoolTime, uint8 HandleIndex);

	bool UseSkill(uint8 SkillNumber);

	void Bind(ABaseCharacter* NewCharacter);
};


UCLASS()
class YUWIBO_API USkillWidget : public UUserWidget
{
	GENERATED_BODY()

	friend class USkillComponent;
private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock;
	UPROPERTY(meta = (BindWidget))
	class UImage* Image;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SecondTextBlock;
	UPROPERTY(meta = (BindWidget))
	class UImage* SecondImage;
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* SecondSkillPanel;
public:

	void SetImage(ABaseCharacter* Character);
};
