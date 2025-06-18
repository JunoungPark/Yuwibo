// Fill out your copyright notice in the Description page of Project Settings.


#include "LocationWidget.h"
#include "Components/Image.h"
void ULocationWidget::SetMapLocation(FVector2D Vector, double Size)
{
	if (MapLocation->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
	{
		auto X = Location.X - 200000;
		auto Y = Location.Y + 200000;

		X /= 200000;
		//X /= 4000;
		X *= 540 * Size;
		//
		Y /= 200000;
		//Y /= 4000;
		Y *= 540 * Size;
		MapLocation->SetRenderTranslation(Vector + FVector2D(X, Y));
	}
	else
	{
		auto X = FMath::Clamp(Location.X - Vector.X, -2500., 2500.);
		auto Y = FMath::Clamp(Location.Y - Vector.Y, -2500., 2500.);
		
		X /= 2500;
		Y /= 2500;

		MiniMapLocation->SetRenderTranslation(FVector2D(X * 140, Y * 140 ));
	}
}

void ULocationWidget::SetActor(AActor* NewActor)
{
	Actor = NewActor;
	Location.X = Actor->GetActorLocation().X;
	Location.Y = Actor->GetActorLocation().Y;
}

void ULocationWidget::SetMiniMapMode(bool IsTrue)
{
	if (IsTrue)
	{
		MapLocation->SetVisibility(ESlateVisibility::Collapsed);

		MiniMapLocation->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		MiniMapLocation->SetVisibility(ESlateVisibility::Collapsed);

		MapLocation->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}
