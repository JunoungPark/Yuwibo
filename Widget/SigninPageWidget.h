// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SigninPageWidget.generated.h"

/**
 * 
 */

UCLASS()
class YUWIBO_API USigninPageWidget : public UUserWidget
{
	GENERATED_BODY()

private:

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* Panel;
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* IDField;
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* PasswordField;
	UPROPERTY(meta = (BindWidget))
	class UButton* ModeChangeButton;
	UPROPERTY(meta = (BindWidget))
	class UBorder* ModeChangeBorder;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ModeChangeText;
	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* SignButton;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SignText;
	UPROPERTY(meta = (BindWidget))
	class UBorder* SignBorder;

private:
	UFUNCTION()
	void OnTextChanged(const FText& Text);
	UFUNCTION()
	void OnSignButtonHovered();
	UFUNCTION()
	void OnSignButtonUnHovered();
	UFUNCTION()
	void OnModeChangeButtonHovered();
	UFUNCTION()
	void OnModeChangeButtonUnHovered();
	UFUNCTION()
	void SignAttempt();
	UFUNCTION()
	void Exit();
protected:
	void NativeOnInitialized()override;
public:
	UFUNCTION()
	void ModeChange();
};
