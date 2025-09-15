// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerController.h"
#include "Yuwibo/Character/BaseCharacter.h"
#include "Yuwibo/Component/ItemComponent.h"
#include "Yuwibo/Component/MapComponent.h"
#include "Yuwibo/Component/HealthComponent.h"
#include "Yuwibo/Component/SkillComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Yuwibo/YuwiboGameModeBase.h"
#include "Yuwibo/Component/MoveComponent.h"
#include "Yuwibo/Widget/TeleportationWidget.h"
#include "Yuwibo/Component/InformationComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Yuwibo/Widget/EndMatchWidget.h"
#include "Yuwibo/Widget/PointWidget.h"
#include "Yuwibo/Character/RockMonster.h"
#include "Yuwibo/Widget/MenuWidget.h"

#ifdef EOS
#include "Online/Lobbies.h"
#include "Online/AuthCommon.h"	
#include "Online/Lobbies.h"
#include "Online/Sessions.h"
#include "Online/OnlineServicesCommon.h"
#include "Online/CoreOnline.h"
#endif

void ABasePlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    ClientPossess(InPawn);

    if (auto Move = Cast<ABaseCharacter>(InPawn)->GetComponent<UMoveComponent>(Component::Move)) Move->SetAttack<false>();

    if (auto Mode = Cast<AYuwiboGameModeBase>(UGameplayStatics::GetGameMode(this)))
    {
        Mode->VisibleStatueWIdget(Cast<ABaseCharacter>(InPawn));
    }

    if (auto RockMonster = Cast<ARockMonster>(InPawn))
    {
        RockMonster->GetComponent<UAnimComponent>(Component::Anim)->SetIsCrouch(false);
    }
}

void ABasePlayerController::OnUnPossess()
{
	Super::OnUnPossess(); 

    ClientUnPossess();
   
}

void ABasePlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority()) return;

    if (auto PW = LoadObject<UClass>(nullptr, TEXT("WidgetBlueprint'/Game/BlueprintClass/HUD/PointWidgetBlueprint.PointWidgetBlueprint_C'")))
    {
        PointWidget = CreateWidget<UPointWidget>(GetWorld(), PW);

        if (PointWidget)
        {
            PointWidget->AddToViewport();
        }

    }
    
    if (auto MW = LoadObject<UClass>(nullptr, TEXT("WidgetBlueprint'/Game/BlueprintClass/HUD/MenuWidgetBlueprint.MenuWidgetBlueprint_C'")))
    {
        MenuWidget = CreateWidget<UMenuWidget>(GetWorld(), MW);

        if (MenuWidget)
        {
            MenuWidget->AddToViewport();
        }

    }

    if (UGameplayStatics::GetCurrentLevelName(GetWorld(), true) != "BattleRoyale_Map_a_WP") return;

    //if (auto LW = LoadObject<UClass>(nullptr, TEXT("WidgetBlueprint'/Game/BlueprintClass/HUD/BP_LoadingBar.BP_LoadingBar_C'")))
    //{
    //    LoadingWidget = CreateWidget<UUserWidget>(GetWorld(), LW);
    //
    //    if (LoadingWidget)
    //    {
    //        LoadingWidget->AddToViewport();
    //    }
    //
    //}
}

void ABasePlayerController::SendPlayerNum_Implementation(uint8 PlayerNum)
{
    if (auto Mode = Cast<AYuwiboGameModeBase>(UGameplayStatics::GetGameMode(this)))
    {
        Mode->SendLobbyNum(PlayerNum);
    }
}

void ABasePlayerController::SendTeleportationLocation_Implementation(uint8 Index)
{
    static_cast<AYuwiboGameModeBase*>(UGameplayStatics::GetGameMode(this))->SetPlayerLocation(this, Index);
}

void ABasePlayerController::GetPlayerNum_Implementation()
{
    UE_LOG(LogTemp, Log, TEXT("FunctionName : GetPlayerNum"));

#ifdef EOS
    using namespace UE::Online;
    FGetJoinedLobbies::Params JoinedParams;
    JoinedParams.LocalAccountId = Cast<UYuwiboGameInstance>(GetGameInstance())->GetAccountID();
    auto JoinedLobbies = GetServices()->GetLobbiesInterface()->GetJoinedLobbies(MoveTemp(JoinedParams));
    if (JoinedLobbies.IsOk() && JoinedLobbies.GetOkValue().Lobbies.Num() && JoinedLobbies.GetOkValue().Lobbies[0]->OwnerAccountId == JoinedParams.LocalAccountId)
    {
        SendPlayerNum(JoinedLobbies.GetOkValue().Lobbies[0]->Members.Num());
    }
#endif EOS

}

void ABasePlayerController::AddViewportWidget_Implementation()
{
    UE_LOG(LogTemp, Log, TEXT("FunctionName : AddViewportWidget"));

    if (auto SSW = LoadObject<UClass>(nullptr, TEXT("WidgetBlueprint'/Game/BlueprintClass/HUD/TeleportationWidgetBlueprint.TeleportationWidgetBlueprint_C'")))
    {
        TeleportationWidget = CreateWidget<UTeleportationWidget>(GetWorld(), SSW);

        if (TeleportationWidget)
        {
            if(LoadingWidget)
            {
                LoadingWidget->RemoveFromParent();  // ������ �θ𿡼� ����
                LoadingWidget = nullptr;
            }

            TeleportationWidget->AddToViewport();

            UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(this, nullptr, EMouseLockMode::DoNotLock, true);
        }
        
    }
}

void ABasePlayerController::GetTeleportationLocation_Implementation()
{
    UE_LOG(LogTemp, Log, TEXT("FunctionName : GetTeleportationLocation"));

    SetShowMouseCursor(false);

    UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);

    if (!TeleportationWidget)
    {
        SendTeleportationLocation(FMath::RandHelper(8));

        return;
    }

    SendTeleportationLocation(TeleportationWidget->GetLocation());
    
    TeleportationWidget->RemoveFromParent();  // ������ �θ𿡼� ����
    TeleportationWidget = nullptr;

}

void ABasePlayerController::ClientPossess_Implementation(APawn* InPawn)
{
    if (auto Char = Cast<ABaseCharacter>(InPawn))
    {
        if (auto Move = Char->GetComponent<UMoveComponent>(Component::Move)) Move->SetAttack<false>();

        if (auto Skill = Char->GetComponent<USkillComponent>(Component::Skill)) Skill->AddViewportWidget();
        if (auto Information = Char->GetComponent<UInformationComponent>(Component::Information)) Information->AddViewportWidget();
        if (auto Item = Char->GetComponent<UItemComponent>(Component::Item)) Item->AddViewportWidget();
        if (auto Map = Char->GetComponent<UMapComponent>(Component::Map)) Map->AddViewportWidget();
        if (auto Health = Char->GetComponent<UHealthComponent>(Component::Health)) Health->AddViewportWidget();

    }

}

void ABasePlayerController::ClientUnPossess_Implementation()
{

    if (auto Char = Cast<ABaseCharacter>(GetPawn()))
    {
        if (auto Skill = Char->GetComponent<USkillComponent>(Component::Skill)) Skill->DestroyWidget();
        if (auto Information = Char->GetComponent<UInformationComponent>(Component::Information)) Information->DestroyWidget();
        if (auto Item = Char->GetComponent<UItemComponent>(Component::Item)) Item->DestroyWidget();
        if (auto Map = Char->GetComponent<UMapComponent>(Component::Map)) Map->DestroyWidget();
        if (auto Health = Char->GetComponent<UHealthComponent>(Component::Health)) Health->DestroyWidget();
    }
}

void ABasePlayerController::AddViewportEndMatchWidget_Implementation(uint8 MaxPlayer, uint8 Rank)
{
    if (EndMatchWidget) return;

    if (auto EW = LoadObject<UClass>(nullptr, TEXT("WidgetBlueprint'/Game/BlueprintClass/HUD/EndMatchWidgetBlueprint.EndMatchWidgetBlueprint_C'")))
    {
        EndMatchWidget = CreateWidget<UEndMatchWidget>(GetWorld(), EW);

        if (EndMatchWidget)
        {
            EndMatchWidget->SetRank(MaxPlayer, Rank);

            EndMatchWidget->AddToViewport();
        }
    }
}

void ABasePlayerController::EndMatch_Implementation(uint8 MaxPlayer, uint8 Rank)
{
    UE_LOG(LogTemp, Log, TEXT("FunctionName : EndMatch"));

    AddViewportEndMatchWidget(MaxPlayer, Rank);

    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABasePlayerController::MatchEnd, 10.f, false);
}

void ABasePlayerController::MatchEnd()
{
    if (auto PC = GetWorld()->GetFirstPlayerController())
    {
        PC->ClientTravel("/Game/BlueprintClass/Level/LobbyLevel", ETravelType::TRAVEL_Absolute);

#ifdef EOS
        using namespace UE::Online;

        if (auto AccountID=Cast<UYuwiboGameInstance>(GetGameInstance())->GetAccountID())
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
#endif
    }
}

void ABasePlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction(TEXT("OnOffMenu"), IE_Pressed, this, &ABasePlayerController::OnOffMenu);
}

void ABasePlayerController::AddPoint_Implementation()
{
    if (PointWidget) PointWidget->AddPoint();
}

void ABasePlayerController::OnOffMenu()
{
    UE_LOG(LogTemp, Log, TEXT("FunctionName : OnOffMenu"));

    if (!MenuWidget) return;

    if (MenuWidget->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
    {
        SetShowMouseCursor(true);

        MenuWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    }
    else
    {
        SetShowMouseCursor(false);

        MenuWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
}
