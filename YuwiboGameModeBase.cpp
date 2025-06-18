// Copyright Epic Games, Inc. All Rights Reserved.


#include "YuwiboGameModeBase.h"
#include "YuwiboPlayerState.h"
#include "Controller/BasePlayerController.h"
#include "ATimerManager.h"
#include "Components/SceneCaptureComponent.h"
#include "Actor/StoneStatue.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Component/MapComponent.h"
#include "Character/BodyChanger.h"
#include "Character/TankLv3.h"
#include "Character/Soldier.h"
#include "Character/Golem.h"
//#include "GolemLv2.h"
//#include "Vampire.h"
//#include "Moose.h"
//#include "Crustaspikan.h"
//#include "CrustaspikanLarvae.h"
//#include "RockMonster.h"
#include "Character/Trainer.h"
#include "Character/Drone.h"
#include "GunHUD.h"
#include "GameFramework/PlayerStart.h"
#include "Component/HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "NavMesh/RecastNavMesh.h"


AYuwiboTestModeBase::AYuwiboTestModeBase()
{
	DefaultPawnClass = ASoldier::StaticClass();
	PlayerControllerClass = ABasePlayerController::StaticClass();
}

AYuwiboLobbyModeBase::AYuwiboLobbyModeBase()
{
	DefaultPawnClass = nullptr;
	//PlayerControllerClass = ABasePlayerController::StaticClass();

	//ConstructorHelpers::FClassFinder<UStartScreenWidget> SSW(TEXT("WidgetBlueprint'/Game/BlueprintClass/HUD/StartScreenWidgetBlueprint.StartScreenWidgetBlueprint_C'"));
	//
	//if (SSW.Succeeded())
	//{
	//
	//	 UUserWidget* Widget = CreateWidget(GetWorld(), SSW.Class);
	//	 UStartScreenWidget* StartScreenWidget = static_cast<UStartScreenWidget*>(Widget);
	//	 if (StartScreenWidget)
	//	 {
	//		 StartScreenWidget->AddToViewport();
	//		 StartScreenWidget->NativeOnInitialized();
	//	 }
	//}
}

AYuwiboGameModeBase::AYuwiboGameModeBase()
{
	DefaultPawnClass = ATank::StaticClass();
	PlayerControllerClass = ABasePlayerController::StaticClass();
	HUDClass = AGunHUD::StaticClass();
	PlayerStateClass = AYuwiboPlayerState::StaticClass();

	TArray<AActor*> AllActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActor);

	for (auto Actor : AllActor)
	{
		if (auto StoneStatue = Cast<AStoneStatue>(Actor))
		{
			Statues.Emplace(StoneStatue);
		}
	}
	//WorldSceneCapture->SetWorldLocation(FVector(196479., -193958., 10000.));

	AvailableCharacters =
	{
		ABodyChanger::StaticClass(),
		//AGolem::StaticClass(),
		ASoldier::StaticClass(),
		ATank::StaticClass(),
		//ATrainer::StaticClass()
	};
}

void AYuwiboGameModeBase::VisibleStoneStatue()
{

	if (!Statues.Num())
	{
		GetWorld()->GetTimerManager().ClearTimer(WorldTimerHandle);
		return;
	}

	
	if (!Alarm)
	{
		for (auto PlayerState : GameState.Get()->PlayerArray)
		{
			static_cast<ABaseCharacter*>(PlayerState.Get()->GetPawn())->VIsibleAlarm();
		}
		Alarm = true;
		return;
	}

	auto i = UKismetMathLibrary::RandomInteger(Statues.Num());
	Statues[i]->SetVisible();
	VisibleStatues.Emplace(Statues[i]);

	for (auto PlayerState : GameState.Get()->PlayerArray)
	{
		static_cast<ABaseCharacter*>(PlayerState.Get()->GetPawn())->GetComponent<UMapComponent>(Component::Map)->AddLocationWidget(Statues[i]);
	}

	Statues.RemoveAt(i);
	Alarm = false;
	
}

void AYuwiboGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (NewPlayer) Cast<ABasePlayerController>(NewPlayer)->GetPlayerNum();

	UE_LOG(LogTemp, Log, TEXT("FunctionName : PostLogin"));
}

void AYuwiboGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	UE_LOG(LogTemp, Log, TEXT("FunctionName : Logout"));

	RemainingPlayer--;

	if (auto Character = Cast<ABaseCharacter>(Exiting->GetPawn()))
		if (Character->GetComponent<UHealthComponent>(Component::Health)->GetCurrentHealth() == 0)RemainingPlayer++;

	if (IsMatchInProgress())
	{
		uint8 i = 0;

		for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			if (auto Character = Cast<ABaseCharacter>(It->Get()->GetPawn()))
			{
				if (Character->GetComponent<UHealthComponent>(Component::Health)->GetCurrentHealth() > 0.f)
				{
					i++;
				}
			}
		}

		if (i <= 1) EndMatch();
	}
}

void AYuwiboGameModeBase::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();

	UE_LOG(LogTemp, Log, TEXT("FunctionName : HandleMatchHasEnded"));

	GetWorld()->GetTimerManager().ClearTimer(WorldTimerHandle);

	for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		Cast<ABasePlayerController>(It->Get())->EndMatch(PlayerNum);
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, []()
		{
			FGenericPlatformMisc::RequestExit(false);
		}, 10.f, false);
}

void AYuwiboGameModeBase::SendLobbyNum_Implementation(uint8 LobbyNum)
{
	PlayerNum = LobbyNum;

	RemainingPlayer = PlayerNum;
}

void AYuwiboGameModeBase::VisibleStatueWIdget(ABaseCharacter* Character)
{
	if (VisibleStatues.Num())Character->GetComponent<UMapComponent>(Component::Map)->AddLocationWidgets(VisibleStatues);
}

void AYuwiboGameModeBase::SetPlayerLocation_Implementation(ABasePlayerController* Controller, uint8 Index)
{
	UE_LOG(LogTemp, Log, TEXT("FunctionName : SetPlayerLocation"));

	TArray<FVector> Vectors =
	{
		 FVector(73140.f, -338270.f, 6670.f),
		 FVector(299640.f, -336250.f, 8410.f),
		 FVector(77510.f,-255590.f,5740.f),
		 FVector(166260.f,-277730.f,5940.f),
		 FVector(338580.f,-227790.f,7910.f),
		 FVector(160910.f,-166930.f,6870.f),
		 FVector(78900.f,-95520.f,7310.f),
		 FVector(272160.f,-116850.f,7380.f),
	};

	int32 RandIdx = FMath::RandRange(0, AvailableCharacters.Num() - 1);

	// 2) �� �ε����� Ŭ���� ��������
	TSubclassOf<APawn> ChosenClass = AvailableCharacters[RandIdx];

	// 3) �ߺ� ������ ���� �迭���� ����
	AvailableCharacters.RemoveAt(RandIdx);

	// 4) ���� & Possess

	if (auto NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
	{
		FVector SpawnLoc;

		FNavLocation RandomLocation;

		for (int i = 0; i < 3; i++)
		{
			if (NavSys->GetRandomReachablePointInRadius(Vectors[Index], 28000, RandomLocation))
			{
				SpawnLoc = RandomLocation.Location + ChosenClass->GetDefaultObject<ACharacter>()->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

				UE_LOG(LogTemp, Log, TEXT("이동한 위치: %s"), *RandomLocation.Location.ToString());

				FRotator SpawnRot = FRotator::ZeroRotator;
				FActorSpawnParameters Params;
				Params.Owner = Controller;

				if (auto NewPawn = GetWorld()->SpawnActor<APawn>(ChosenClass, SpawnLoc, SpawnRot, Params))
				{
					Controller->Possess(NewPawn);

					break;
				}
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Player ReachablePoint Fail"));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("이동 가능한 위치를 찾지 못했습니다."));
	}

	
}

void AYuwiboGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	TimerManagerInstance = GetWorld()->SpawnActor<AATimerManager>(AATimerManager::StaticClass());

	GetWorld()->GetTimerManager().SetTimer(WorldTimerHandle, this, &AYuwiboGameModeBase::VisibleStoneStatue, 30.f, true);

	UNavigationSystemV1* Nav = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!Nav)
	{
		UE_LOG(LogTemp, Warning, TEXT("NavigationSystemV1 not found."));
	}

	const ANavigationData* NavData = Nav->GetDefaultNavDataInstance(FNavigationSystem::ECreateIfEmpty::DontCreate);
	if (!NavData)
	{
		UE_LOG(LogTemp, Warning, TEXT("NavData not found (not loaded)."));
	}

	if (const ARecastNavMesh* RecastNav = Cast<ARecastNavMesh>(NavData))
	{
		const int32 TileCount = RecastNav->GetNavMeshTilesCount();

		UE_LOG(LogTemp, Warning, TEXT("NavMesh Tile Count: %d"), TileCount);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("RecastNavMesh not found."));
	}

	if (!Nav->IsNavigationBuilt(GetWorld()->GetWorldSettings()))
	{
		UE_LOG(LogTemp, Warning, TEXT("NavMesh exists but not built."));
	}

}

void AYuwiboGameModeBase::PlayerIsEnd(ABasePlayerController* Controller)
{
	Controller->AddViewportEndMatchWidget(PlayerNum, RemainingPlayer--);
	
	uint8 i = 0;

	for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (auto Character = Cast<ABaseCharacter>(It->Get()->GetPawn()))
		{
			if (Character->GetComponent<UHealthComponent>(Component::Health)->GetCurrentHealth() > 0.f)
			{
				i++;
			}
		}
	}

	if (i <= 1) EndMatch();
}

void AYuwiboGameModeBase::CheckPlayerNum()
{

	if (RemainingPlayer != 0 && RemainingPlayer == GetNumPlayers())
	{
		for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			Cast<ABasePlayerController>(It->Get())->AddViewportWidget();
		}

		GetWorld()->GetTimerManager().ClearTimer(StartMatchHandle);

		GetWorld()->GetTimerManager().SetTimer(StartMatchHandle, [this]() {

			UE_LOG(LogTemp, Log, TEXT("FunctionName : GameStart"));

			for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
			{
				Cast<ABasePlayerController>(It->Get())->GetTeleportationLocation();
			}

			//GetWorld()->GetTimerManager().SetTimer(WorldTimerHandle, this, &AYuwiboGameModeBase::VisibleStoneStatue, 180.f, true);
			//GetWorld()->GetTimerManager().SetTimer(WorldTimerHandle, this, &AYuwiboGameModeBase::VisibleStoneStatue, 30.f, true);

			}, 10.f, false);

		UE_LOG(LogTemp, Log, TEXT("FunctionName : GameStartTimer"));
	}
}

void AYuwiboGameModeBase::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	GetWorld()->GetTimerManager().SetTimer(StartMatchHandle, this, &AYuwiboGameModeBase::CheckPlayerNum, 0.1f, true);

    //UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    //if (!NavSys) return;
	//
    //TArray<FVector> SpawnLocations;
	//constexpr float MinDistance = 300.0f;
	//constexpr float SearchRadius = 2000.0f;
    //FNavLocation NavLocation;
	//
    //for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    //{
    //    APlayerController* PC = It->Get();
    //    if (!PC || PC->GetPawn() != nullptr) continue;
	//
    //    bool bValidLocationFound = false;
    //    for (uint32 Attempt = 0; Attempt < 10; ++Attempt)
    //    {
    //        if (NavSys->GetRandomPointInNavigableRadius(FVector::ZeroVector, SearchRadius, NavLocation))
    //        {
    //            FVector PotentialLocation = NavLocation.Location;
	//
    //            bool bIsFarEnough = true;
    //            for (const FVector& ExistingLocation : SpawnLocations)
    //            {
    //                if (FVector::Dist(PotentialLocation, ExistingLocation) < MinDistance)
    //                {
    //                    bIsFarEnough = false;
    //                    break;
    //                }
    //            }
	//
    //            if (bIsFarEnough)
    //            {
    //                bValidLocationFound = true;
    //                SpawnLocations.Add(PotentialLocation);
    //                break;
    //            }
    //        }
    //    }
    //}
}