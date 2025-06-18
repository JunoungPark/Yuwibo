// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GLauncherHUD.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API AGLauncherHUD : public AHUD
{
	GENERATED_BODY()

	virtual void DrawHUD() override;
};
