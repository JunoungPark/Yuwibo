// Fill out your copyright notice in the Description page of Project Settings.


#include "YuwiboGameInstance.h"	
#include "Widget/LobbyBrowserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Networking/NetworkingSubsystem.h"

#ifdef EOS
#include "Online/AuthCommon.h"	
#include "Online/Lobbies.h"
#include "Online/Sessions.h"
#include "Online/OnlineServicesCommon.h"
#include "Online/CoreOnline.h"
#endif

UYuwiboGameInstance::UYuwiboGameInstance()
{
	ConstructorHelpers::FObjectFinder<UDataTable> DT(TEXT("DataTable'/Game/BlueprintClass/DataTable/CharacterDataTable.CharacterDataTable'"));
	if (DT.Succeeded())
	{
		CharacterDataTable = DT.Object;
	}

	ConstructorHelpers::FObjectFinder<UDataTable> DTB(TEXT("DataTable'/Game/BlueprintClass/DataTable/ItemDataTable.ItemDataTable'"));
	if (DTB.Succeeded())
	{
		ItemDataTable = DTB.Object;
	}
}

#ifdef EOS

//void UYuwiboGameInstance::LoginClient()
//{
//
//
//
//
//    if (UGameplayStatics::GetCurrentLevelName(GetWorld(), true) != "LobbyLevel") return;
//
//    UGameplayStatics::GetPlayerController(this, 0)->SetShowMouseCursor(true);
//
//	using namespace UE::Online;
//    
//    FAuthLogin::Params Params;
//    Params.PlatformUserId = FPlatformUserId::CreateFromInternalId(0);
//    Params.CredentialsType = LoginCredentialsType::AccountPortal;
//    Params.CredentialsToken.Set<FString>(TEXT(""));
//    
//    GetServices()->GetAuthInterface()->Login(MoveTemp(Params)).OnComplete([this](const TOnlineResult<FAuthLogin>& LoginResult)
//        {
//            if (LoginResult.IsOk())
//            {
//                // �α��� ���� -> ExchangeCode �߱� �õ�
//                AccountID = LoginResult.GetOkValue().AccountInfo.Get().AccountId;
//                if (auto SSW = LoadObject<UClass>(nullptr, TEXT("WidgetBlueprint'/Game/BlueprintClass/HUD/LobbyBrowserWidgetBlueprint.LobbyBrowserWidgetBlueprint_C'")))
//                {
//                    if (auto Widget = CreateWidget<ULobbyBrowserWidget>(GetWorld(), SSW))
//                    {
//                        Widget->AddToViewport();
//                    }
//                }
//    
//            }
//            else
//            {
//                FGenericPlatformMisc::RequestExit(false);
//    
//                const FOnlineError& Error = LoginResult.GetErrorValue();
//                UE_LOG(LogTemp, Error, TEXT("Login Failed: %s"), *Error.GetLogString());
//            }
//    
//    
//        });
//
//
//
//
//
//
//
//}

//void UYuwiboGameInstance::LoginServer_Implementation(FAccountId AccountId)
//{
//	using namespace UE::Online;
//
//	GetServices()->GetAuthInterface()->QueryExternalServerAuthTicket(FAuthQueryExternalServerAuthTicket::Params{ AccountId })
//		.OnComplete([this](const TOnlineResult<FAuthQueryExternalServerAuthTicket>& Result)
//			{
//				if (Result.IsOk())
//				{
//					IAuthPtr Interface = GetServices()->GetAuthInterface();
//
//					FAuthLogin::Params Params;
//					Params.PlatformUserId = FPlatformUserId();
//					Params.CredentialsType = LoginCredentialsType::ExchangeCode;
//					Params.CredentialsToken.Emplace<FString>(FBase64::Encode(Result.GetOkValue().ExternalServerAuthTicket.Data));
//
//					Interface->Login(MoveTemp(Params)).OnComplete([this](const TOnlineResult<FAuthLogin>& LoginResult)
//						{
//							if (LoginResult.IsOk())
//							{
//								AccountID = LoginResult.GetOkValue().AccountInfo->AccountId;
//								const TSharedRef<FAccountInfo> AccountInfo = LoginResult.GetOkValue().AccountInfo;
//								// ���� AccountInfo ������Ʈ�� �׼����� �� �ֽ��ϴ�.
//							}
//							else
//							{
//								FOnlineError Error = LoginResult.GetErrorValue();
//								// ���� ������ ó���� �� �ֽ��ϴ�.
//							}
//						});
//				}
//			});
//}

//void UYuwiboGameInstance::GetExchangeCode(TFunction<void(const FString&)> OnExchangeCodeReceived)
//{
//	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
//
//	FString URL = TEXT("https://api.epicgames.dev/epic/oauth/v1/token");
//	Request->SetURL(URL);
//	Request->SetVerb(TEXT("POST"));
//	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
//
//	FString PostData = TEXT("grant_type=client_credentials")
//		TEXT("&client_id=YOUR_CLIENT_ID")
//		TEXT("&client_secret=YOUR_CLIENT_SECRET");
//	Request->SetContentAsString(PostData);
//
//	Request->OnProcessRequestComplete().BindLambda([OnExchangeCodeReceived](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
//		{
//			if (bWasSuccessful && Response.IsValid())
//			{
//				TSharedPtr<FJsonObject> JsonObject;
//				TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
//
//				if (FJsonSerializer::Deserialize(Reader, JsonObject))
//				{
//					FString ExchangeCode = JsonObject->GetStringField("exchange_code");
//					UE_LOG(LogTemp, Log, TEXT("Exchange Code: %s"), *ExchangeCode);
//					OnExchangeCodeReceived(ExchangeCode); 
//				}
//			}
//			else
//			{
//				UE_LOG(LogTemp, Error, TEXT("Failed to get Exchange Code!"));
//			}
//		});
//
//	Request->ProcessRequest();
//}

//void UYuwiboGameInstance::OnStart()
//{
//    Super::OnStart();
//
//    if (IsDedicatedServerInstance()) return;
//
//    GetSubsystem<UNetworkingSubsystem>()->OnPostWorldInit();
//
//	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
//		{
//			if (auto* PC = GetWorld()->GetFirstPlayerController())
//			{
//				PC->bShowMouseCursor = true;
//				FInputModeUIOnly InputMode;
//				PC->SetInputMode(InputMode);
//			}
//		});
//}

//void UYuwiboGameInstance::Shutdown()
//{
//	Super::Shutdown();
//
//	using namespace UE::Online;
//
//	if (!AccountID.IsValid()) return;
//    // ���� ������
//    FGetAllSessions::Params SessionParams;
//    SessionParams.LocalAccountId = AccountID;
//    if (GetServices()->GetSessionsInterface()->GetAllSessions(MoveTemp(SessionParams)).IsOk())
//    {
//        FLeaveSession::Params LeaveParams;
//        LeaveParams.LocalAccountId = AccountID;
//        LeaveParams.SessionName = "Game";
//        LeaveParams.bDestroySession = false;
//
//        GetServices()->GetSessionsInterface()->LeaveSession(MoveTemp(LeaveParams));
//    }
//
//    // �κ� ������
//    FGetJoinedLobbies::Params JoinedParams;
//    JoinedParams.LocalAccountId = AccountID;
//    auto JoinedLobbies = GetServices()->GetLobbiesInterface()->GetJoinedLobbies(MoveTemp(JoinedParams));
//    if (JoinedLobbies.IsOk() && JoinedLobbies.GetOkValue().Lobbies.Num())
//    {
//        FLeaveLobby::Params LeaveParams;
//        LeaveParams.LobbyId = JoinedLobbies.GetOkValue().Lobbies[0]->LobbyId;
//        LeaveParams.LocalAccountId = AccountID;
//
//        GetServices()->GetLobbiesInterface()->LeaveLobby(MoveTemp(LeaveParams));
//    }
//
//    // �α׾ƿ�
//    if (GetServices()->GetAuthInterface()->IsLoggedIn(AccountID))
//    {
//        FAuthLogout::Params Params;
//        Params.bDestroyAuth = true;
//        Params.LocalAccountId = AccountID;
//
//        GetServices()->GetAuthInterface()->Logout(MoveTemp(Params));
//    }
//
//}
#endif

FYuwiboCharacterData* UYuwiboGameInstance::GetCharacterData(FName CharacterName)
{
	return CharacterDataTable->FindRow<FYuwiboCharacterData>(CharacterName, "");
}



FYuwiboItemData* UYuwiboGameInstance::GetItemData(uint8 ItemID)
{
	return ItemDataTable->FindRow<FYuwiboItemData>(FName(*FString::FromInt(ItemID)), "");
}
