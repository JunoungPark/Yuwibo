// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Online/AuthCommon.h"	
#include "Online/Lobbies.h"
#include "Online/Sessions.h"
#include "Online/OnlineServicesCommon.h"
#include "Online/CoreOnline.h"
#include "Yuwibo/YuwiboGameInstance.h"

void UMenuWidget::Exit()
{
    if (auto PC = GetWorld()->GetFirstPlayerController())
    {
        PC->ClientTravel("/Game/BlueprintClass/Level/LobbyLevel", ETravelType::TRAVEL_Absolute); 
        
        using namespace UE::Online;

        if (auto AccountID = Cast<UYuwiboGameInstance>(GetGameInstance())->GetAccountID())
            // ���� ������
        {
            FGetAllSessions::Params SessionParams;
            SessionParams.LocalAccountId = AccountID;
            if (GetServices()->GetSessionsInterface()->GetAllSessions(MoveTemp(SessionParams)).IsOk())
            {
                FLeaveSession::Params LeaveParams;
                LeaveParams.LocalAccountId = AccountID;
                LeaveParams.SessionName = "Game";
                LeaveParams.bDestroySession = false;

                GetServices()->GetSessionsInterface()->LeaveSession(MoveTemp(LeaveParams));
            }

            // �κ� ������
            FGetJoinedLobbies::Params JoinedParams;
            JoinedParams.LocalAccountId = AccountID;
            auto JoinedLobbies = GetServices()->GetLobbiesInterface()->GetJoinedLobbies(MoveTemp(JoinedParams));
            if (JoinedLobbies.IsOk() && JoinedLobbies.GetOkValue().Lobbies.Num())
            {
                FLeaveLobby::Params LeaveParams;
                LeaveParams.LobbyId = JoinedLobbies.GetOkValue().Lobbies[0]->LobbyId;
                LeaveParams.LocalAccountId = AccountID;

                GetServices()->GetLobbiesInterface()->LeaveLobby(MoveTemp(LeaveParams));
            }
        }

    }
}

void UMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ExitButton->OnClicked.AddDynamic(this, &UMenuWidget::Exit);

    ExitButton->OnHovered.AddDynamic(this, &UMenuWidget::OnButtonHovered);
    ExitButton->OnUnhovered.AddDynamic(this, &UMenuWidget::OnButtonUnHovered);
}

void UMenuWidget::OnButtonHovered()
{
    ExitText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))); 
    FSlateFontInfo FontInfo = ExitText->GetFont();
    FontInfo.Size = 60; // 원하는 크기로 설정
    ExitText->SetFont(FontInfo);
}

void UMenuWidget::OnButtonUnHovered()
{
    ExitText->SetColorAndOpacity(FSlateColor(FLinearColor(0.75f, 0.75f, 0.75f, 1.0f)));  
    FSlateFontInfo FontInfo = ExitText->GetFont();
    FontInfo.Size = 48; // 원하는 크기로 설정
    ExitText->SetFont(FontInfo);
}
