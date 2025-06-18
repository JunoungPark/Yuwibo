// Fill out your copyright notice in the Description page of Project Settings.


#include "StoneStatue.h"
#include "Yuwibo/Character/BaseCharacter.h"	
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Yuwibo/YuwiboGameModeBase.h"
#include "Yuwibo/Component/MapComponent.h"
#include "Yuwibo/YuwiboPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Yuwibo/Controller/BasePlayerController.h"
// Sets default values
AStoneStatue::AStoneStatue()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetActorHiddenInGame(true);
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("StaticMesh'/Game/Infinity_Blade_Assets/Meshes/Props/BetrayalStatue.BetrayalStatue'"));
	if (SM.Succeeded())
	{
		StaticMesh->SetStaticMesh(SM.Object);
		StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &AStoneStatue::OverlapEvent);
		StaticMesh->SetCollisionProfileName(FName("NoCollision"));
	}

	SetReplicates(true);
}

void AStoneStatue::OverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;

	if (auto Character = Cast<ABaseCharacter>(OtherActor))
	{
		if (!Character->IsPlayerControlled()) return;

		if (auto PlayerState = Cast<AYuwiboPlayerState>(Character->GetPlayerState()))
		{
			StaticMesh->SetCollisionProfileName(FName("NoCollision"));

			PlayerState->AddScore();
			Cast<ABasePlayerController>(Character->GetController())->AddPoint();

			UE_LOG(LogTemp, Log, TEXT("%f"), PlayerState->GetScore());

			static_cast<AYuwiboGameModeBase*>(UGameplayStatics::GetGameMode(this))->RemoveVisibleStatue(this);
		
			for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
			{
				if (auto Char = Cast<ABaseCharacter>(It->Get()->GetPawn()))
					if (auto Map = Char->GetComponent<UMapComponent>(Component::Map)) Map->RemoveLocationWidget(this);
			}

			SetActorTickEnabled(true);
		}
	}
}

void AStoneStatue::OnVisible_Implementation()
{
	SetActorHiddenInGame(false);
}

void AStoneStatue::OnRep_DissolveAmount()
{
	for (int32 i = 0; i < StaticMesh->GetNumMaterials(); i++)
	{
		if (auto DynMat = StaticMesh->CreateDynamicMaterialInstance(i))
		{
			DynMat->SetScalarParameterValue(FName("Process"), DissolveAmount);
		}
	}
}

void AStoneStatue::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AStoneStatue, DissolveAmount);
}

// Called when the game starts or when spawned
void AStoneStatue::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);
}

// Called every frame
void AStoneStatue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority()) return;

	if (DissolveAmount < 1.5f)
	{
		DissolveAmount += DeltaTime * 0.5f;

		if (DissolveAmount > 1.5f) Destroy();

	}
}

void AStoneStatue::SetVisible()
{
	OnVisible();

	StaticMesh->SetCollisionProfileName(FName("OverlapAll"));
}

