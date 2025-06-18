// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyBrowserWidget.h"
#include "Online/OnlineServices.h"
#include "Online/CoreOnline.h"
#include "Online/Auth.h"
#include "Components/TextBlock.h"
#include "Components/ListView.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"	
#include "Online/Sessions.h"	
#include "OnlineServicesCommon/Public/Online/SessionsCommon.h"
#include "Kismet/GameplayStatics.h"
#include "Online/UserInfo.h"
#include "SocketSubsystem.h"
void ULobbyBrowserWidget::SetLobbyEntry(ULobbyEntryWidget& Entry, UE::Online::FLobby Lobby)
{
    Entry.LobbyName = FText::FromString(Lobby.Attributes.Find(FName("HostName"))->GetString());
    
    Entry.PlayersCurrentNum = Lobby.Members.Num();
    Entry.PlayersMaxNum = Lobby.MaxMembers;

    Entry.LobbyID = Lobby.LobbyId;
   // Entry.Lobby_Name->SetText();
}

void ULobbyBrowserWidget::OnItemDoubleClicked(UObject* MyListView)
{
    JoinLobby(Cast<ULobbyEntryWidget>(MyListView)->GetLobbyID());
}

void ULobbyBrowserWidget::JoinLobby(UE::Online::FLobbyId LobbyID)
{
    UE::Online::ILobbiesPtr Lobbies = UE::Online::GetServices()->GetLobbiesInterface();

    UE::Online::FJoinLobby::Params JoinParams;
    JoinParams.LocalAccountId = GetAccountID();
    JoinParams.LobbyId = LobbyID; // �Ǵ� CreateLobby ������� ���� LobbyId

    Lobbies->JoinLobby(MoveTemp(JoinParams)).OnComplete([this](const UE::Online::TOnlineResult<UE::Online::FJoinLobby>& Result)
        {
            if (Result.IsOk())
            {
                const UE::Online::FLobby& Lobby = *Result.GetOkValue().Lobby.Get();
                //UE_LOG(LogTemp, Log, TEXT(" �κ� ���� ����! LobbyId: %s"), Lobby.LobbyId);

                CreateLobbyWidget();

                UpdateLobby();
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT(" �κ� ���� ����: %s"), *Result.GetErrorValue().GetLogString());
            }
        });
}

void ULobbyBrowserWidget::CreateLobby()
{
    if (HostName->GetText().IsEmptyOrWhitespace())
    {
        return;
    }

    UE::Online::ILobbiesPtr Lobbies = UE::Online::GetServices()->GetLobbiesInterface();

    UE::Online::FCreateLobby::Params CreateParams;
    CreateParams.LocalAccountId = GetAccountID();
    CreateParams.SchemaId = TEXT("GameLobby");
    CreateParams.MaxMembers = 3; // �Ǵ� CreateLobby ������� ���� LobbyId
    CreateParams.JoinPolicy = UE::Online::ELobbyJoinPolicy::PublicAdvertised;

    CreateParams.Attributes.Emplace(UE::Online::FSchemaAttributeId(TEXT("HostName")), UE::Online::FSchemaVariant(HostName->GetText().ToString()));

    Lobbies->CreateLobby(MoveTemp(CreateParams)).OnComplete([this](const UE::Online::TOnlineResult<UE::Online::FCreateLobby>& Result)
        {
            if (Result.IsOk())
            {
                CreateLobbyWidget();

                TArray<TSharedPtr<FJsonValue>> JsonValues;

                TArray<FString> OrderedPlayers = {
                    ToLogString(GetAccountID()),"","","","",
                };

                for (const FString& Item : OrderedPlayers)
                {
                    JsonValues.Add(MakeShared<FJsonValueString>(Item));
                }

                // JSON �迭�� ���ڿ��� ����ȭ
                FString OutputString;
                TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
                FJsonSerializer::Serialize(JsonValues, Writer);

                using namespace UE::Online;

                FModifyLobbyAttributes::Params Params;
                Params.LobbyId = Result.GetOkValue().Lobby->LobbyId;
                Params.LocalAccountId = GetAccountID();
                Params.UpdatedAttributes.Emplace(FSchemaAttributeId(TEXT("LobbyOrders")), FSchemaVariant(OutputString));

                GetServices()->GetLobbiesInterface()->ModifyLobbyAttributes(MoveTemp(Params)).OnComplete([this](const UE::Online::TOnlineResult<UE::Online::FModifyLobbyAttributes>& Result)
                    {
                        UpdateLobby();
                    });


                UE_LOG(LogTemp, Warning, TEXT("�κ� ���� ����!")); 

                //UE_LOG(LogTemp, Log, TEXT(" �κ� ���� ����! LobbyId: %s"), Lobby.LobbyId);
                UpdateEntry();
            }
            else
            {
                UE_LOG(LogTemp, Log, TEXT("lobby create fail"));
            }
        }); 

    Cancel();
}

void ULobbyBrowserWidget::Cancel()
{
    HostGame->SetVisibility(ESlateVisibility::Collapsed);
}

void ULobbyBrowserWidget::GameExit()
{
    if (auto World = GetWorld())
    {
        UKismetSystemLibrary::QuitGame(World, UGameplayStatics::GetPlayerController(World, 0), EQuitPreference::Quit, false);
    }
}

void ULobbyBrowserWidget::UpdateLobby()
{
    using namespace UE::Online; 
    FGetJoinedLobbies::Params Params;

    Params.LocalAccountId = GetAccountID();

    auto Lobby = GetServices()->GetLobbiesInterface()->GetJoinedLobbies(MoveTemp(Params)).GetOkValue().Lobbies[0];

    LobbyWidget->HostName->SetText(FText::FromString(Lobby->Attributes.Find(FName("HostName"))->GetString()));

    TArray<FString> OrderedPlayers;

    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Lobby->Attributes.Find(FSchemaAttributeId(TEXT("LobbyOrders")))->GetString());
    TArray<TSharedPtr<FJsonValue>> JsonValues;

    if (FJsonSerializer::Deserialize(Reader, JsonValues))
    {
        for (const TSharedPtr<FJsonValue>& Value : JsonValues)
        {
            if (Value->Type == EJson::String)
            {
                OrderedPlayers.Emplace(Value->AsString());
            }
        }
    }

    TArray<UTextBlock*> TextBlocks =
    {
        LobbyWidget->FirstMember,
        LobbyWidget->SecondMember,
        LobbyWidget->ThirdMember,
    };

    auto Members = Lobby->Members;

    int i = 0;

    for (const FString& OrderedPlayer : OrderedPlayers)
    {
        if (OrderedPlayer.IsEmpty()) continue;

        for (const auto& Member : Members)
        {
            if (OrderedPlayer == ToLogString(Member.Key))
            {
                if (auto UserInfo = GetServices()->GetUserInfoInterface())
                {
                    FGetUserInfo::Params UserParams;
                    UserParams.AccountId = Member.Key;
                    UserParams.LocalAccountId = GetAccountID();

                    auto User = UserInfo->GetUserInfo(MoveTemp(UserParams));

                    TextBlocks[i]->SetText(FText::FromString(User.GetOkValue().UserInfo->DisplayName));

                }
                i++;

                Members.Remove(Member.Key);

                break; // ��ġ�ϴ� Member ã������ ���� OrderedPlayer�� �̵�
            }
        }
    }

    if(Lobby->OwnerAccountId == GetAccountID())
    {
        LobbyWidget->StartGameButton->SetIsEnabled(true);

        LobbyWidget->StartGameButton->SetRenderOpacity(1.f);
    }
    else
    {
        LobbyWidget->StartGameButton->SetIsEnabled(false);

        LobbyWidget->StartGameButton->SetRenderOpacity(0.1f);
    }
}

void ULobbyBrowserWidget::CreateLobbyWidget()
{
    if (LobbyWidget)    LobbyWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    else if (auto SSW = LoadObject<UClass>(nullptr, TEXT("WidgetBlueprint'/Game/BlueprintClass/HUD/LobbyWidgetBlueprint.LobbyWidgetBlueprint_C'")))
    {
        LobbyWidget = CreateWidget<ULobbyWidget>(GetWorld(), SSW);

        if (LobbyWidget) LobbyWidget->AddToViewport();
        
    }
}

void ULobbyBrowserWidget::UpdateEntry()
{

    UE::Online::ILobbiesPtr Lobbies = UE::Online::GetServices()->GetLobbiesInterface();

    UE::Online::FFindLobbies::Params FindParams;
    FindParams.LocalAccountId = GetAccountID();

    Lobbies->FindLobbies(MoveTemp(FindParams)).OnComplete([this](const UE::Online::TOnlineResult<UE::Online::FFindLobbies>& Result)
        {
            if (Result.IsOk())
            {
                if (!IsValid(this) || !IsValid(Lobby_List))
                {
                    return;
                }
                auto LobbiesFound = Result.GetOkValue().Lobbies; 
                
                LobbiesFound.Sort([](const TSharedRef<const UE::Online::FLobby>& A, const TSharedRef<const UE::Online::FLobby>& B)
                    {
                        const UE::Online::FSchemaVariant* NameA = A->Attributes.Find(UE::Online::FSchemaAttributeId(TEXT("HostName")));
                        const UE::Online::FSchemaVariant* NameB = B->Attributes.Find(UE::Online::FSchemaAttributeId(TEXT("HostName")));

                        FString StringA = NameA ? NameA->GetString() : TEXT("");
                        FString StringB = NameB ? NameB->GetString() : TEXT("");

                        return StringA != StringB ?  StringA < StringB : A->Members.Num() > B->Members.Num();;
                    });

                UE_LOG(LogTemp, Log, TEXT("ã�� �κ� ��: %d"), LobbiesFound.Num());

                TArray<UObject*> NewListData;

                for (const auto Lobby : LobbiesFound)
                {

                    if (!LobbyNameText->GetText().IsEmptyOrWhitespace())
                    {
                        if (Lobby.Get().Attributes.Find(FName("HostName"))->GetString().Contains(LobbyNameText->GetText().ToString()));
                        else if (Lobby.Get().JoinPolicy != UE::Online::ELobbyJoinPolicy::InvitationOnly) continue;
                        else continue;
                    }
                    auto Entry = NewObject<ULobbyEntryWidget>(this, ULobbyEntryWidget::StaticClass());
                    SetLobbyEntry(*Entry, Lobby.Get());
                    NewListData.Emplace(Entry);

                    //UE_LOG(LogTemp, Log, TEXT(": %s,: %d / %d"), Lobby.Get().Members.Num(), Lobby.Get().MaxMembers);
                }
                Lobby_List->SetListItems(NewListData);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("%s"), *Result.GetErrorValue().GetLogString());
            }
        });
}

void ULobbyBrowserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    using namespace UE::Online;

    ExitButton->OnClicked.AddDynamic(this, &ULobbyBrowserWidget::GameExit);

    auto AttrChangedHandle = GetServices()->GetLobbiesInterface()->OnLobbyAttributesChanged().Add([this](const FLobbyAttributesChanged& Data)
        {
            if (auto ID = Data.AddedAttributes.Find(FSchemaAttributeId(TEXT("GameSessionId"))))
            {

                FJoinSession::Params Params;
                Params.LocalAccountId = GetAccountID();

                TArray<uint8> RepData;
                FTCHARToUTF8 Utf8(*ID->GetString());
                RepData.Append((uint8*)Utf8.Get(), Utf8.Length());

                Params.SessionId = FOnlineSessionIdStringRegistry(Data.Lobby->LobbyId.GetOnlineServicesType()).FromReplicationData(RepData);

                GetServices()->GetSessionsInterface()->JoinSession(MoveTemp(Params));

            }

            if (Data.ChangedAttributes.Find(FSchemaAttributeId(TEXT("LobbyOrders"))))
            {
                UpdateLobby();
            }

            if (auto ID = Data.AddedAttributes.Find(FSchemaAttributeId(TEXT("URL"))))
            {
                UGameplayStatics::GetPlayerController(GetWorld(), 0)->ClientTravel(FString::Printf(TEXT("%s"), *ID->GetString()), TRAVEL_Absolute);
            }
        });

    AttrChangedHandle = GetServices()->GetLobbiesInterface()->OnLobbyMemberJoined().Add([this](const FLobbyMemberJoined& Event)
        {
            if (Event.Lobby->OwnerAccountId != GetAccountID())
            {
                return;
            }

            TArray<FString> OrderedPlayers;

            TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Event.Lobby->Attributes.Find(FSchemaAttributeId(TEXT("LobbyOrders")))->GetString());
            TArray<TSharedPtr<FJsonValue>> JsonValues;

            if (FJsonSerializer::Deserialize(Reader, JsonValues))
            {
                for (const TSharedPtr<FJsonValue>& Value : JsonValues)
                {
                    if (Value->Type == EJson::String)
                    {
                        OrderedPlayers.Emplace(Value->AsString());
                    }
                }
            }

            for (auto& OrderedPlayer : OrderedPlayers)
            {
                if (OrderedPlayer.IsEmpty())
                {
                    OrderedPlayer = ToLogString(Event.Member->AccountId);

                    break;
                }
            }

            JsonValues.Empty();
            for (const FString& Item : OrderedPlayers)
            {
                JsonValues.Add(MakeShared<FJsonValueString>(Item));
            }

            // JSON �迭�� ���ڿ��� ����ȭ
            FString OutputString;
            TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
            FJsonSerializer::Serialize(JsonValues, Writer);

            FModifyLobbyAttributes::Params Params;
            Params.LobbyId = Event.Lobby->LobbyId;
            Params.LocalAccountId = GetAccountID();
            Params.UpdatedAttributes.Emplace(FSchemaAttributeId(TEXT("LobbyOrders")), FSchemaVariant(OutputString));
    
            GetServices()->GetLobbiesInterface()->ModifyLobbyAttributes(MoveTemp(Params));
        });

    AttrChangedHandle = GetServices()->GetLobbiesInterface()->OnLobbyMemberLeft().Add([this](const FLobbyMemberLeft& Event)
        {
            if (Event.Lobby->OwnerAccountId != GetAccountID())
            {
                return;
            }

            TArray<FString> OrderedPlayers;

            TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Event.Lobby->Attributes.Find(FSchemaAttributeId(TEXT("LobbyOrders")))->GetString());
            TArray<TSharedPtr<FJsonValue>> JsonValues;

            if (FJsonSerializer::Deserialize(Reader, JsonValues))
            {
                for (const TSharedPtr<FJsonValue>& Value : JsonValues)
                {
                    if (Value->Type == EJson::String)
                    {
                        OrderedPlayers.Emplace(Value->AsString());
                    }
                }
            }

            for (auto& OrderedPlayer : OrderedPlayers)
            {
                if (OrderedPlayer==ToLogString(Event.Member->AccountId))
                {
                    OrderedPlayer.Empty();

                    break;
                }
            }

            JsonValues.Empty();
            for (const FString& Item : OrderedPlayers)
            {
                JsonValues.Add(MakeShared<FJsonValueString>(Item));
            }

            // JSON �迭�� ���ڿ��� ����ȭ
            FString OutputString;
            TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
            FJsonSerializer::Serialize(JsonValues, Writer);

            FModifyLobbyAttributes::Params Params;
            Params.LobbyId = Event.Lobby->LobbyId;
            Params.LocalAccountId = GetAccountID();
            Params.UpdatedAttributes.Emplace(FSchemaAttributeId(TEXT("LobbyOrders")), FSchemaVariant(OutputString));

            GetServices()->GetLobbiesInterface()->ModifyLobbyAttributes(MoveTemp(Params));
        });

    LobbyNameText->OnTextChanged.AddDynamic(this, &ULobbyBrowserWidget::FindLobby);
    Lobby_List->OnItemDoubleClicked().AddUFunction(this, "OnItemDoubleClicked");

    HostGameButton->OnClicked.AddDynamic(this, &ULobbyBrowserWidget::CreateLobby);
    CancelButton->OnClicked.AddDynamic(this, &ULobbyBrowserWidget::Cancel); 
    
    UpdateEntry();
}

void ULobbyEntryWidget::SetData(ULobbyEntryWidget* Widget)
{
    Lobby_Name->SetText(Widget->LobbyName);
    PlayersNum->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), Widget->PlayersCurrentNum, Widget->PlayersMaxNum)));
    LobbyID = Widget->LobbyID;

}

void ULobbyEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
    SetData(static_cast<ULobbyEntryWidget*>(ListItemObject));
}

void ULobbyWidget::GameStart()
{
    using namespace UE::Online;

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
}

void ULobbyWidget::LeaveLobby()
{
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
}

void ULobbyWidget::OnStartButtonHovered()
{
    StartGame->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))); 
    FSlateFontInfo FontInfo = StartGame->GetFont();
    FontInfo.Size = 42; // 원하는 크기로 설정
    StartGame->SetFont(FontInfo);
}

void ULobbyWidget::OnStartButtonUnHovered()
{
    StartGame->SetColorAndOpacity(FSlateColor(FLinearColor(0.75f, 0.75f, 0.75f, 1.0f))); 
    FSlateFontInfo FontInfo = StartGame->GetFont();
    FontInfo.Size = 35; // 원하는 크기로 설정
    StartGame->SetFont(FontInfo);
}

void ULobbyWidget::OnExitButtonHovered()
{
    ExitLobby->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)));
    FSlateFontInfo FontInfo = StartGame->GetFont();
    FontInfo.Size = 42; // 원하는 크기로 설정
    ExitLobby->SetFont(FontInfo);
}

void ULobbyWidget::OnExitButtonUnHovered()
{
    ExitLobby->SetColorAndOpacity(FSlateColor(FLinearColor(0.75f, 0.75f, 0.75f, 1.0f)));
    FSlateFontInfo FontInfo = StartGame->GetFont();
    FontInfo.Size = 35; // 원하는 크기로 설정
    ExitLobby->SetFont(FontInfo);
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
    LeaveLobbyButton->OnHovered.AddDynamic(this, &ULobbyWidget::OnExitButtonHovered);
    LeaveLobbyButton->OnUnhovered.AddDynamic(this, &ULobbyWidget::OnExitButtonUnHovered);
    LeaveLobbyButton->OnPressed.AddDynamic(this, &ULobbyWidget::OnExitButtonUnHovered);
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
