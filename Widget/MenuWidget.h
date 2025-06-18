// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ExitText;

private:
	UFUNCTION()
	void OnButtonHovered();

	UFUNCTION()
	void OnButtonUnHovered();

	UFUNCTION()
	void Exit();

protected:
	void NativeOnInitialized()override;


};
