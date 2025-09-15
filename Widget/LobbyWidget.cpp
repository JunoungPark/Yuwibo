// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"
#include "Kismet/GameplayStatics.h"
#include "SocketSubsystem.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Yuwibo/Networking/NetworkingSubsystem.h"

void ULobbyWidget::GameStart()
{
    using namespace UE::Online;
#ifdef EOS
    //FGetJoinedLobbies::Params CurrentLobbyParams;
    //
    //CurrentLobbyParams.LocalAccountId = GetAccountID();
    //if (GetServices()->GetLobbiesInterface()->GetJoinedLobbies(MoveTemp(CurrentLobbyParams)).GetOkValue().Lobbies[0]->Members.Num() < 2) return;
    FCreateSession::Params Params;

    Params.LocalAccountId = GetAccountID();
    Params.SessionName = "Game";
    Params.bPresenceEnabled = true;
    Params.SessionSettings.SchemaName = "GameSession";
    Params.SessionSettings.NumMaxConnections = 3;

    GetServices()->GetSessionsInterface()->CreateSession(MoveTemp(Params)).OnComplete([this](const TOnlineResult<FCreateSession>& Result)
        {
            if (Result.IsOk())
            {

                FGetJoinedLobbies::Params LobbyParams;

                LobbyParams.LocalAccountId = GetAccountID();

                FModifyLobbyAttributes::Params Params;
                Params.LobbyId = GetServices()->GetLobbiesInterface()->GetJoinedLobbies(MoveTemp(LobbyParams)).GetOkValue().Lobbies[0]->LobbyId;

                FModifyLobbyJoinPolicy::Params PolicyParams;
                PolicyParams.LocalAccountId = GetAccountID();
                PolicyParams.LobbyId = Params.LobbyId;
                PolicyParams.JoinPolicy = ELobbyJoinPolicy::InvitationOnly;

                GetServices()->GetLobbiesInterface()->ModifyLobbyJoinPolicy(MoveTemp(PolicyParams));

                Params.LocalAccountId = GetAccountID();

                FGetAllSessions::Params SessionParams;
                SessionParams.LocalAccountId = GetAccountID();

                auto SessionID = GetServices()->GetSessionsInterface()->GetAllSessions(MoveTemp(SessionParams)).GetOkValue().Sessions[0]->GetSessionId();
                Params.UpdatedAttributes.Emplace(FSchemaAttributeId(TEXT("GameSessionId")), FSchemaVariant(FOnlineSessionIdStringRegistry(SessionID.GetOnlineServicesType()).ToLogString(SessionID)));


                FString ServerExecutablePath = FPaths::Combine(FPaths::LaunchDir(), TEXT("../WindowsServer/Yuwibo/Binaries/Win64/YuwiboServer-Win64-Shipping.exe"));
                //FString ServerExecutablePath = FPaths::Combine(FPaths::ProjectDir(), TEXT("Binaries/Win64/YuwiboServer.exe"));
                int32 ServerPort = 7777;
                FString MapName = TEXT("/Game/BattleRoyaleStarterKit/Maps/BattleRoyale_Map_a/BattleRoyale_Map_a_WP");
                //FString MapName = TEXT("/Game/BlueprintClass/Level/GameLevel");

                FString CommandLineArgs = FString::Printf(TEXT("%s -log -port=%d"), *MapName, ServerPort);

                // ���� ����
                FPlatformProcess::CreateProc(
                    *ServerExecutablePath,
                    *CommandLineArgs,
                    true,
                    false,
                    false,
                    nullptr,
                    0,
                    nullptr,
                    nullptr
                );

                // ���� URL ����
                bool bCanBind = false;

                FString ServerURL = FString::Printf(TEXT("%s:%d"), *ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, bCanBind)->ToString(false), ServerPort);

                // ��: "192.168.0.23:7777"
                Params.UpdatedAttributes.Emplace(FSchemaAttributeId(TEXT("URL")), FSchemaVariant(ServerURL));

                GetServices()->GetLobbiesInterface()->ModifyLobbyAttributes(MoveTemp(Params));

                UGameplayStatics::GetPlayerController(GetWorld(), 0)->ClientTravel(FString::Printf(TEXT("%s"), *ServerURL), TRAVEL_Absolute);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("%s"), *Result.GetErrorValue().GetLogString());
            }
        });
#endif

    FString ServerExecutablePath = FPaths::Combine(FPaths::LaunchDir(), TEXT("../WindowsServer/Yuwibo/Binaries/Win64/YuwiboServer-Win64-Shipping.exe"));
    //FString ServerExecutablePath = FPaths::Combine(FPaths::ProjectDir(), TEXT("Binaries/Win64/YuwiboServer.exe"));
    int32 ServerPort = 7777;
    FString MapName = TEXT("/Game/BattleRoyaleStarterKit/Maps/BattleRoyale_Map_a/BattleRoyale_Map_a_WP");
    //FString MapName = TEXT("/Game/BlueprintClass/Level/GameLevel");

    FString CommandLineArgs = FString::Printf(TEXT("%s -log -port=%d"), *MapName, ServerPort);

    // ���� ����
    FPlatformProcess::CreateProc(
        *ServerExecutablePath,
        *CommandLineArgs,
        true,
        false,
        false,
        nullptr,
        0,
        nullptr,
        nullptr
    );

    // ���� URL ����
    bool bCanBind = false;

    FString ServerURL = FString::Printf(TEXT("%s:%d"), *ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, bCanBind)->ToString(false), ServerPort);

    // ��: "192.168.0.23:7777"
    if(GetGameInstance()->GetSubsystem<UNetworkingSubsystem>()->StartGame(TCHAR_TO_UTF8(*ServerURL))) UGameplayStatics::GetPlayerController(GetWorld(), 0)->ClientTravel(ServerURL, TRAVEL_Absolute);
}

void ULobbyWidget::LeaveLobby()
{
#ifdef EOS
    SetVisibility(ESlateVisibility::Collapsed);

    using namespace UE::Online;

    FGetJoinedLobbies::Params JoinedParams;
    JoinedParams.LocalAccountId = GetAccountID();
    auto JoinedLobbies = GetServices()->GetLobbiesInterface()->GetJoinedLobbies(MoveTemp(JoinedParams));
    if (JoinedLobbies.IsOk() && JoinedLobbies.GetOkValue().Lobbies.Num())
    {
        FLeaveLobby::Params LeaveParams;
        LeaveParams.LobbyId = JoinedLobbies.GetOkValue().Lobbies[0]->LobbyId;
        LeaveParams.LocalAccountId = GetAccountID();

        GetServices()->GetLobbiesInterface()->LeaveLobby(MoveTemp(LeaveParams));
    }
#endif
    SetVisibility(ESlateVisibility::Collapsed);

    GetGameInstance()->GetSubsystem<UNetworkingSubsystem>()->LeaveRoom();
}

void ULobbyWidget::OnStartButtonHovered()
{
    StartGameBorder->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void ULobbyWidget::OnStartButtonUnHovered()
{
    StartGameBorder->SetVisibility(ESlateVisibility::Collapsed);
}

void ULobbyWidget::OnLeaveLobbyButtonHovered()
{
    LeaveLobbyBorder->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void ULobbyWidget::OnLeaveLobbyButtonUnHovered()
{
    LeaveLobbyBorder->SetVisibility(ESlateVisibility::Collapsed);
}

void ULobbyWidget::NativeOnInitialized()
{
    FirstMemberButton;
    SecondMemberButton;
    ThirdMemberButton;
    StartGameButton->OnClicked.AddDynamic(this, &ULobbyWidget::GameStart);
    StartGameButton->OnHovered.AddDynamic(this, &ULobbyWidget::OnStartButtonHovered);
    StartGameButton->OnUnhovered.AddDynamic(this, &ULobbyWidget::OnStartButtonUnHovered);
    StartGameButton->OnPressed.AddDynamic(this, &ULobbyWidget::OnStartButtonUnHovered);
    StartGameButton->OnReleased.AddDynamic(this, &ULobbyWidget::OnStartButtonHovered);

    LeaveLobbyButton->OnClicked.AddDynamic(this, &ULobbyWidget::LeaveLobby);
    LeaveLobbyButton->OnHovered.AddDynamic(this, &ULobbyWidget::OnLeaveLobbyButtonHovered);
    LeaveLobbyButton->OnUnhovered.AddDynamic(this, &ULobbyWidget::OnLeaveLobbyButtonUnHovered);
    LeaveLobbyButton->OnPressed.AddDynamic(this, &ULobbyWidget::OnLeaveLobbyButtonUnHovered);
    LeaveLobbyButton->OnReleased.AddDynamic(this, &ULobbyWidget::OnStartButtonHovered);
}

FReply ULobbyWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.GetEffectingButton() != EKeys::RightMouseButton)  return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

    TArray<UButton*> ButtonsToCheck = {
             FirstMemberButton,
             SecondMemberButton,
             ThirdMemberButton,
    };

    auto Position = InMouseEvent.GetScreenSpacePosition();

    for (auto& Button : ButtonsToCheck)
    {
        if (Button && Button->GetCachedGeometry().IsUnderLocation(Position))
        {
            return FReply::Handled();
        }
    }

    return FReply::Unhandled();
}

void ULobbyWidget::UpdateLobby(const UnrealEngineMessage::RoomInfo& Room, const google::protobuf::RepeatedPtrField<UnrealEngineMessage::UserInfo>& Users, const std::string MyID)
{
    HostName->SetText(FText::FromString(UTF8_TO_TCHAR(Room.room_name().c_str())));

    TArray<UTextBlock*> TextBlocks =
    {
        FirstMember,
        SecondMember,
        ThirdMember,
    };

    int i = 0;

    for (auto& User : Users)
    {
        TextBlocks[i]->SetText(FText::FromString(UTF8_TO_TCHAR(User.user_name().c_str())));

        i++;
    }

    if (MyID == Room.host_id())
    {
        StartGameButton->SetIsEnabled(true);

        StartGameButton->SetRenderOpacity(1.f);
    }
    else
    {
        StartGameButton->SetIsEnabled(false);

        StartGameButton->SetRenderOpacity(0.1f);
    }
}