// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportationWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

void UTeleportationWidget::SetText()
{

	if (Count <= 0) GetWorld()->GetTimerManager().ClearTimer(Timer);
	else RemainingTime->SetText(FText::FromString(FString::Printf(TEXT("%d초후 게임이 시작됩니다. 스폰 위치를 설정하세요."), Count)));
	
	Count--;

}

void UTeleportationWidget::SetLocation()
{
	TArray<UButton*> Buttons =
	{
		 FirstSpawnLocation,
		 SecondSpawnLocation,
		 ThirdSpawnLocation,
		 FourthSpawnLocation,
		 FifthSpawnLocation,
		 SixthSpawnLocation,
		 SeventhSpawnLocation,
		 EighthSpawnLocation
	};

	for (int i = 0; i < Buttons.Num(); i++)
	{
		FButtonStyle Style = Buttons[i]->GetStyle();

		FLinearColor MyColor(0.24081f, 0.495466f, 0.0f, 0.3f); // RGBA

		if (Buttons[i]->IsPressed())
		{
			Style.Normal.TintColor = FSlateColor(MyColor);

			Buttons[i]->SetStyle(Style);

			ButtonIndex = i;

			continue;
		}

		if (Style.Normal.TintColor != FLinearColor(0.495466f, 0.f, 0.f, 0.3f))
		{
			Style.Normal.TintColor = FSlateColor(FLinearColor(0.495466f, 0.f, 0.f, 0.3f));

			Buttons[i]->SetStyle(Style);
		}


	}

}

void UTeleportationWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	TArray<UButton*> Buttons =
	{
		 FirstSpawnLocation,
		 SecondSpawnLocation,
		 ThirdSpawnLocation,
		 FourthSpawnLocation,
		 FifthSpawnLocation,
		 SixthSpawnLocation,
		 SeventhSpawnLocation,
		 EighthSpawnLocation
	};

	for(auto& Button : Buttons)
	{
		Button->OnPressed.AddDynamic(this, &UTeleportationWidget::SetLocation);
		
		FLinearColor MyColor(0.24081f, 0.495466f, 0.0f, 0.4f); // RGBA

		FButtonStyle NewStyle = Button->GetStyle();
		NewStyle.Normal.TintColor = FSlateColor(FLinearColor(0.495466f, 0.f, 0.f, 0.3f));
		NewStyle.Hovered.TintColor = FSlateColor(MyColor * 1.1f); // 약간 밝게
		NewStyle.Pressed.TintColor = FSlateColor(MyColor * 0.9f); // 약간 어둡게

		Button->SetStyle(NewStyle);
	}

	UGameplayStatics::GetPlayerController(this, 0)->SetShowMouseCursor(true);

	GetWorld()->GetTimerManager().SetTimer(Timer, this, &UTeleportationWidget::SetText, 1.f, true, 0.f);
}

FString UTeleportationWidget::GetText()
{
	return RemainingTime->GetText().ToString();
}

uint8 UTeleportationWidget::GetLocation()
{
	TArray<UButton*> Buttons =
	{
		 FirstSpawnLocation,
		 SecondSpawnLocation,
		 ThirdSpawnLocation,
		 FourthSpawnLocation,
		 FifthSpawnLocation,
		 SixthSpawnLocation,
		 SeventhSpawnLocation,
		 EighthSpawnLocation
	};

	return ButtonIndex != -1 ? ButtonIndex : FMath::RandHelper(Buttons.Num());
	 
}
