// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserNameWidget.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API UUserNameWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* UserNameField;
	UPROPERTY(meta = (BindWidget))
	class UButton* RegisterButton;
	UPROPERTY(meta = (BindWidget))
	class UBorder* RegisterBorder;
private:
	UFUNCTION()
	void OnTextChanged(const FText& Text);
	UFUNCTION()
	void OnRegisterButtonHovered();
	UFUNCTION()
	void OnRegisterButtonUnHovered();
	UFUNCTION()
	void Register();
protected:
	void NativeOnInitialized()override;
	
};
