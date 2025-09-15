// Fill out your copyright notice in the Description page of Project Settings.


#include "PopupWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UPopupWidget::OnExitButtonHovered()
{
    ExitText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
}

void UPopupWidget::OnExitButtonUnHovered()
{
    ExitText->SetColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
}

void UPopupWidget::Exit()
{
    SetVisibility(ESlateVisibility::Collapsed);
}

void UPopupWidget::NativeOnInitialized()
{
    ExitButton->OnHovered.AddDynamic(this, &UPopupWidget::OnExitButtonHovered);
    ExitButton->OnUnhovered.AddDynamic(this, &UPopupWidget::OnExitButtonUnHovered);
    ExitButton->OnClicked.AddDynamic(this, &UPopupWidget::Exit);
}


void UPopupWidget::OnSigninResponse(bool success, UnrealEngineMessage::ErrorCode errcode)
{
    if (success) NotificationMessage->SetText(FText::FromString(TEXT("로그인 완료")));
    else
    {
        switch (errcode)
        {
        case UnrealEngineMessage::UNKNOWN:
            NotificationMessage->SetText(FText::FromString(TEXT("로그인 실패")));
            break;
        case UnrealEngineMessage::INVALID_REQUEST:
            NotificationMessage->SetText(FText::FromString(TEXT("ID 또는 비밀번호가 존재하지 않습니다.")));
            break;
        case UnrealEngineMessage::ROOM_NOT_FOUND:
            NotificationMessage->SetText(FText::FromString(TEXT("로그인 실패")));
            break;
        case UnrealEngineMessage::USER_ALREADY_EXISTS:
            NotificationMessage->SetText(FText::FromString(TEXT("로그인 실패")));
            break;
        default:
            NotificationMessage->SetText(FText::FromString(TEXT("로그인 실패")));
            break;
        }
    }
}

void UPopupWidget::OnSignupResponse(bool success, UnrealEngineMessage::ErrorCode errcode)
{
    if (success) NotificationMessage->SetText(FText::FromString(TEXT("회원가입 완료")));
    else
    {
        switch (errcode)
        {
        case UnrealEngineMessage::UNKNOWN:
            NotificationMessage->SetText(FText::FromString(TEXT("회원가입 실패")));
            break;
        case UnrealEngineMessage::INVALID_REQUEST:
            NotificationMessage->SetText(FText::FromString(TEXT("회원가입 실패")));
            break;
        case UnrealEngineMessage::ROOM_NOT_FOUND:
            NotificationMessage->SetText(FText::FromString(TEXT("회원가입 실패")));
            break;
        case UnrealEngineMessage::USER_ALREADY_EXISTS:
            NotificationMessage->SetText(FText::FromString(TEXT("이미 존재하는 ID 입니다.")));
            break;
        default:
            NotificationMessage->SetText(FText::FromString(TEXT("회원가입 실패")));
            break;
        }
    }
}

void UPopupWidget::OnJoinRoomResponse(UnrealEngineMessage::ErrorCode errcode)
{
    switch (errcode)
    {
    case UnrealEngineMessage::UNKNOWN:
        NotificationMessage->SetText(FText::FromString(TEXT("참가 실패")));
        break;
    case UnrealEngineMessage::INVALID_REQUEST:
        NotificationMessage->SetText(FText::FromString(TEXT("방이 가득차있거나 존재하지 않습니다.")));
        break;
    case UnrealEngineMessage::ROOM_NOT_FOUND:
        NotificationMessage->SetText(FText::FromString(TEXT("참가 실패")));
        break;
    case UnrealEngineMessage::USER_ALREADY_EXISTS:
        NotificationMessage->SetText(FText::FromString(TEXT("참가 실패")));
        break;
    default:
        NotificationMessage->SetText(FText::FromString(TEXT("참가 실패")));
        break;
    }
}
