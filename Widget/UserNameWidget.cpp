// Fill out your copyright notice in the Description page of Project Settings.


#include "UserNameWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Yuwibo/Networking/NetworkingSubsystem.h"

void UUserNameWidget::OnTextChanged(const FText& Text)
{
    if (UserNameField->GetText().IsEmptyOrWhitespace())
    {
        RegisterButton->SetIsEnabled(false);

        RegisterButton->SetRenderOpacity(0.5f);

        RegisterBorder->SetVisibility(ESlateVisibility::Collapsed);

    }
    else
    {
        RegisterButton->SetIsEnabled(true);

        RegisterButton->SetRenderOpacity(1.f);
    }
}

void UUserNameWidget::OnRegisterButtonHovered()
{
    if (UserNameField->GetText().IsEmptyOrWhitespace()) return;

    RegisterBorder->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UUserNameWidget::OnRegisterButtonUnHovered()
{
    RegisterBorder->SetVisibility(ESlateVisibility::Collapsed);
}

void UUserNameWidget::Register()
{
    if (UserNameField->GetText().IsEmptyOrWhitespace()) return;

    GetGameInstance()->GetSubsystem<UNetworkingSubsystem>()->Register(TCHAR_TO_UTF8(*UserNameField->GetText().ToString()));
}

void UUserNameWidget::NativeOnInitialized()
{
    UserNameField->OnTextChanged.AddDynamic(this, &UUserNameWidget::OnTextChanged);
    RegisterButton->OnClicked.AddDynamic(this, &UUserNameWidget::Register);

    RegisterButton->OnHovered.AddDynamic(this, &UUserNameWidget::OnRegisterButtonHovered);
    RegisterButton->OnUnhovered.AddDynamic(this, &UUserNameWidget::OnRegisterButtonUnHovered);

    OnTextChanged(FText::GetEmpty());
}
