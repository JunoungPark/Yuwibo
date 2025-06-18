// Fill out your copyright notice in the Description page of Project Settings.


#include "EndMatchWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Online/AuthCommon.h"	
#include "Online/Lobbies.h"
#include "Online/Sessions.h"
#include "Online/OnlineServicesCommon.h"
#include "Online/CoreOnline.h"
#include "Yuwibo/YuwiboGameInstance.h"

void UEndMatchWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ExitButton->OnClicked.AddDynamic(this, &UEndMatchWidget::Exit);
}

void UEndMatchWidget::NativeConstruct()
{
    PlayAnimation(EndMatchAnimation);
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	PlayerController->SetShowMouseCursor(true);
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerController);
}

void UEndMatchWidget::SetRank(uint8 MaxPlayer, uint8 Rank)
{
	RankText->SetText(FText::FromString(FString::Printf(TEXT("You Placed %d of %d "), Rank, MaxPlayer)));
}

void UEndMatchWidget::Exit()
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
