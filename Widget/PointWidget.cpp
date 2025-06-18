// Fill out your copyright notice in the Description page of Project Settings.


#include "PointWidget.h"
#include "Components/Image.h"
void UPointWidget::AddPoint_Implementation()
{
	TArray<UImage*> Images =
	{
		FirstPoint,
		SecondPoint,
		ThirdPoint
	};

	for (auto& Image : Images)
	{
		if (Image->GetVisibility() != ESlateVisibility::HitTestInvisible)
		{
			Image->SetVisibility(ESlateVisibility::HitTestInvisible);
			
			return;
		}
	}
	
}