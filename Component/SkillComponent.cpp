// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Yuwibo/YuwiboGameInstance.h"
#include "kismet/GameplayStatics.h"
#include "Yuwibo/Character/BodyChanger.h"

void USkillWidget::SetImage(ABaseCharacter* Character)
{
	if (auto MyGameInstance = Cast<UYuwiboGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		if (auto Data = MyGameInstance->GetCharacterData(Character->GetCharacterName()))
		{
			Image->SetBrushFromSoftTexture(Data->SkillTexture[0].LoadSynchronous());

			if (Character->IsA(ABodyChanger::StaticClass()))
			{
				SecondImage->SetBrushFromSoftTexture(Data->SkillTexture[1].LoadSynchronous()); 
				SecondSkillPanel->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
		}
	}
}


// Sets default values for this component's properties
USkillComponent::USkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	
	SetIsReplicatedByDefault(true);
	// ...
}

void USkillComponent::OnRep_CanSkill()
{
	if (!Skill) return;

	UpdateSkillUI(Skill->Image, Skill->TextBlock, CanSkill, CoolTime, 0);
}

void USkillComponent::OnRep_CanSecondSkill()
{
	if (!Skill) return;

	UpdateSkillUI(Skill->SecondImage, Skill->SecondTextBlock, CanSecondSkill, SecondCoolTime, 1);
}

void USkillComponent::UpdateSkillUI(UImage* Image, UTextBlock* TextBlock, bool bCanUse, float InCoolTime, uint8 HandleIndex)
{
	if (!Skill) return;

	auto Brush = Image->GetBrush();

	if (bCanUse)
	{   // Brush ����

		Brush.TintColor = FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f)); // Tint ����

		TextBlock->SetText(FText::FromString(""));

		GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle[HandleIndex]);

	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle[HandleIndex], this, HandleIndex ? &USkillComponent::CanSecondSkillTimeCount : &USkillComponent::CanSkillTimeCount, 1.0f, true);

		Brush.TintColor = FSlateColor(FLinearColor(0.f, 0.f, 0.f, 0.4f)); // Tint ����

		TextBlock->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), InCoolTime)));
	}

	Image->SetBrush(Brush);
}

void USkillComponent::TimeCount(UTextBlock* TextBlock)
{

	auto Time = FCString::Atoi(*TextBlock->GetText().ToString()) - 1;

	if (Time <= 0) return;

	TextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), Time)));

}

void USkillComponent::CanSkillTimeCount()
{
	if (!Skill) return;

	TimeCount(Skill->TextBlock);
}

void USkillComponent::CanSecondSkillTimeCount()
{
	if (!Skill) return;

	TimeCount(Skill->SecondTextBlock);
}

void USkillComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USkillComponent, CanSkill);

	DOREPLIFETIME(USkillComponent, CanSecondSkill);
}
// Called when the game starts
void USkillComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Character->IsLocallyControlled()) AddViewportWidget();
	// ...
	
}

void USkillComponent::DestroyWidget()
{
	if (Skill)
	{
		Skill->RemoveFromParent();  // ������ �θ𿡼� ����
		Skill = nullptr;             // ���۷��� �ʱ�ȭ
	}
}

void USkillComponent::AddViewportWidget()
{
	if (Skill) return;

	if (auto SUW = LoadObject<UClass>(nullptr, TEXT("WidgetBlueprint'/Game/BlueprintClass/HUD/SkillWidgetBlueprint.SkillWidgetBlueprint_C'")))
	{
		Skill = Cast<USkillWidget>(CreateWidget(GetWorld(), SUW));

		if (Skill)
		{
			Skill->AddToViewport(-1);

			Skill->SetImage(Character);

			GetCoolTime();
		}
		
	}
}
void USkillComponent::GetCoolTime_Implementation()
{
	double RemainingTime = -1.f;

	double SecondRemainingTime = -1.f;

	if (GetWorld()->GetTimerManager().IsTimerActive(CooldownTimerHandle[0]))
	{
		RemainingTime = GetWorld()->GetTimerManager().GetTimerRemaining(CooldownTimerHandle[0]);
	}

	if(GetWorld()->GetTimerManager().IsTimerActive(CooldownTimerHandle[1]))
	{
		SecondRemainingTime = GetWorld()->GetTimerManager().GetTimerRemaining(CooldownTimerHandle[1]);
	}

	TimerStart(RemainingTime, SecondRemainingTime);
}

void USkillComponent::TimerStart_Implementation(double RemainingTime, double SecondRemainingTime)
{
	if (RemainingTime > 0)
	{
		StartTimer(Skill->Image, Skill->TextBlock, CanSkill, RemainingTime, 0);
	}

	if (SecondRemainingTime > 0)
	{
		StartTimer(Skill->SecondImage, Skill->SecondTextBlock, CanSecondSkill, SecondRemainingTime, 1);
	}

}

void USkillComponent::StartTimer(UImage* Image, UTextBlock* TextBlock, bool bCanUse, float InCoolTime, uint8 HandleIndex)
{
	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle[HandleIndex], this, HandleIndex ? &USkillComponent::CanSecondSkillTimeCount : &USkillComponent::CanSkillTimeCount, 1.0f, true, FMath::Frac(InCoolTime));

	auto Brush = Skill->SecondImage->GetBrush();

	Brush.TintColor = FSlateColor(FLinearColor(0.f, 0.f, 0.f, 0.4f)); // Tint ����

	Skill->SecondTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), InCoolTime)));

	Skill->SecondImage->SetBrush(Brush);
}

bool USkillComponent::UseSkill(uint8 SkillNumber)
{
	if (SkillNumber == 0 && CanSkill)
	{
		CanSkill = false;

		GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle[0], this, &USkillComponent::SetCanSkill, CoolTime);
		return true;
	}
	else if (SkillNumber == 1 && CanSecondSkill)
	{
		CanSecondSkill = false;

		GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle[1], this, &USkillComponent::SetCanSecondSkill, SecondCoolTime);
		return true;
	}

	return false;

}
void USkillComponent::Bind(ABaseCharacter* NewCharacter)
{
	Character = NewCharacter;
}

