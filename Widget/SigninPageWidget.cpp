// Fill out your copyright notice in the Description page of Project Settings.


#include "SigninPageWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/EditableTextBox.h"
#include "Yuwibo/Networking/NetworkingSubsystem.h"

void USigninPageWidget::OnTextChanged(const FText& Text)
{
    if (IDField->GetText().IsEmptyOrWhitespace() || PasswordField->GetText().IsEmptyOrWhitespace())
    {
        SignButton->SetIsEnabled(false);

        SignButton->SetRenderOpacity(0.5f);

        SignBorder->SetVisibility(ESlateVisibility::Collapsed);

    }
    else
    {
        SignButton->SetIsEnabled(true);

        SignButton->SetRenderOpacity(1.f);
    }
}

void USigninPageWidget::OnSignButtonHovered()
{
    SignBorder->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void USigninPageWidget::OnSignButtonUnHovered()
{
    SignBorder->SetVisibility(ESlateVisibility::Collapsed);
}

void USigninPageWidget::OnModeChangeButtonHovered()
{
    ModeChangeBorder->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void USigninPageWidget::OnModeChangeButtonUnHovered()
{
    ModeChangeBorder->SetVisibility(ESlateVisibility::Collapsed);
}

void USigninPageWidget::SignAttempt()
{
    if (IDField->GetText().IsEmptyOrWhitespace() || PasswordField->GetText().IsEmptyOrWhitespace()) return;

    if (SignText->GetText().ToString() == "Sign in") GetGameInstance()->GetSubsystem<UNetworkingSubsystem>()->Signin(TCHAR_TO_UTF8(*IDField->GetText().ToString()), TCHAR_TO_UTF8(*PasswordField->GetText().ToString()));
    else GetGameInstance()->GetSubsystem<UNetworkingSubsystem>()->Signup(TCHAR_TO_UTF8(*IDField->GetText().ToString()), TCHAR_TO_UTF8(*PasswordField->GetText().ToString()));
}

void USigninPageWidget::ModeChange()
{
    if (SignText->GetText().ToString() == "Sign in")
    {
        SignText->SetText(FText::FromString("Sign up"));
        ModeChangeText->SetText(FText::FromString("Sign in"));
    }
    else
    {
        SignText->SetText(FText::FromString("Sign in"));
        ModeChangeText->SetText(FText::FromString("Sign up"));
    }

    IDField->SetText(FText::GetEmpty());
    PasswordField->SetText(FText::GetEmpty());

    OnTextChanged(FText::GetEmpty());
}

void USigninPageWidget::Exit()
{
    UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
}

void USigninPageWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    IDField->OnTextChanged.AddDynamic(this, &USigninPageWidget::OnTextChanged);
    PasswordField->OnTextChanged.AddDynamic(this, &USigninPageWidget::OnTextChanged);

    SignButton->OnClicked.AddDynamic(this, &USigninPageWidget::SignAttempt);

    SignButton->OnHovered.AddDynamic(this, &USigninPageWidget::OnSignButtonHovered);
    SignButton->OnUnhovered.AddDynamic(this, &USigninPageWidget::OnSignButtonUnHovered);
    ModeChangeButton->OnHovered.AddDynamic(this, &USigninPageWidget::OnModeChangeButtonHovered);
    ModeChangeButton->OnUnhovered.AddDynamic(this, &USigninPageWidget::OnModeChangeButtonUnHovered);

    ModeChangeButton->OnClicked.AddDynamic(this, &USigninPageWidget::ModeChange);
    ExitButton->OnClicked.AddDynamic(this, &USigninPageWidget::Exit);

    OnTextChanged(FText::GetEmpty());

}