// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBox.h"
#include "Components/BoxComponent.h"
#include "Yuwibo/Character/BaseCharacter.h"
#include "Yuwibo/Animinstance/ItemBoxAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Yuwibo/Character/RockMonster.h"
#include "NavigationSystem.h"
#include "WorldPartition/WorldPartition.h"
#include "Components/CapsuleComponent.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "NavMesh/RecastNavMesh.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "WorldPartition/WorldPartition.h"
#include "WorldPartition/WorldPartitionRuntimeCell.h"
#include "WorldPartition/WorldPartitionSubsystem.h"
#include "Engine/World.h"
#include "WorldPartition/WorldPartitionHandle.h"
#include "WorldPartition/WorldPartitionRuntimeSpatialHash.h"
#include "WorldPartition/WorldPartitionLevelHelper.h"
#include "WorldPartition/WorldPartitionHelpers.h"
// Sets default values
AItemBox::AItemBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxComponent->SetBoxExtent(FVector(250., 270., 110.));
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AItemBox::OverlapPlayer);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AItemBox::OverlapEndPlayer);
	BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");

	ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/DynamicChest_Fx_Pack/Sources/Animations/Chest_11/SKM_Chest_11.SKM_Chest_11'"));
	if (SM.Succeeded())
	{
		SkeletalMesh->SetSkeletalMesh(SM.Object);
		SkeletalMesh->SetupAttachment(BoxComponent);
		SkeletalMesh->SetRelativeLocation(FVector(0., 0., -110.));
		SkeletalMesh->SetCollisionProfileName("BlockAllDynamic");
		ConstructorHelpers::FClassFinder<UItemBoxAnimInstance> AB(TEXT("AnimBlueprint'/Game/BlueprintClass/ItemBox/ItemBoxAB.ItemBoxAB_C'"));
		if (AB.Succeeded())
		{
			SkeletalMesh->SetAnimInstanceClass(AB.Class);
		}
	}
	
}

void AItemBox::OnRep_OpenCount()
{
	if (OpenCount <= 0)
		Cast<UItemBoxAnimInstance>(SkeletalMesh->GetAnimInstance())->SetIsOpen(false);
	else
		Cast<UItemBoxAnimInstance>(SkeletalMesh->GetAnimInstance())->SetIsOpen(true);
}

void AItemBox::SpawnMonster()
{
	for (int j = 0; j < 2;)
	{
		if (auto NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
		{
			constexpr float SearchRadius = 1000.0f; // 탐색 반경

			FNavLocation ResultLocation;

			for (int k = 0; k < 3; k++)
			{
				if (NavSys->GetRandomReachablePointInRadius(GetActorLocation(), SearchRadius, ResultLocation))
				{
					FVector SpawnLocation = ResultLocation.Location + ARockMonster::StaticClass()->GetDefaultObject<ACharacter>()->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

					FRotator SpawnRotation(0.0f, 0.0f, 0.0f);

					if (auto Enemy = GetWorld()->SpawnActor<ARockMonster>(ARockMonster::StaticClass(), SpawnLocation, SpawnRotation))
					{
						UE_LOG(LogTemp, Log, TEXT("uu %s"), *Enemy->GetName());

						UE_LOG(LogTemp, Log, TEXT("ReachablePoint Success %f,%f.%f"), SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z);

						j++;

						break;
					}
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("NavMesh에서 유효한 위치를 찾을 수 없습니다."));

			return;
		}

	}

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

}

void AItemBox::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItemBox, OpenCount);

}

// Called when the game starts or when spawned
void AItemBox::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority()) return;

	int i = 0;
	for (; i < UKismetMathLibrary::RandomIntegerInRange(2, 4); i++)
	{
		FItemData NewItem;
		NewItem.ItemID = UKismetMathLibrary::RandomIntegerInRange(1, 10);
		Item.Emplace(NewItem);
	}

	for (; i < 8; i++)
	{
		FItemData NewItem;
		NewItem.ItemID = 0; 
		Item.Emplace(NewItem);
	}

	UE_LOG(LogTemp, Log, TEXT("ItemBox %s Location : %f,%f.%f"), *GetName(), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AItemBox::SpawnMonster, 5.f, true);
}

void AItemBox::OverlapPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherComp->IsA(UShapeComponent::StaticClass()) || !HasAuthority()) return;

	if (auto Character = Cast<ABaseCharacter>(OtherActor))
	{
		if (UItemComponent* ItemComponent = Character->GetComponent<UItemComponent>(Component::Item))
		{
			ItemComponent->AddItemHolder(this);
		}
		//Character->SetCanGetItem(true);
		//Character->AddItemHolder(this);
	}
}

void AItemBox::OverlapEndPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->IsA(UShapeComponent::StaticClass()) || !HasAuthority()) return;

	if (auto Character = Cast<ABaseCharacter>(OtherActor))
	{
		if (UItemComponent* ItemComponent = Character->GetComponent<UItemComponent>(Component::Item))
		{
			ItemComponent->RemoveItemHolder(this);
		}
		//Character->SetCanGetItem(false);
		//Character->RemoveItemHolder(this);
	}
}

void AItemBox::Open(UItemComponent* ItemComponent)
{
	OpenCount++;
	ConnectPlayer.Emplace(ItemComponent);
	ItemComponent->OpenItemWindowAndHolder(Item);
	
}

void AItemBox::Close(UItemComponent* ItemComponent)
{
	ConnectPlayer.Remove(ItemComponent);
	OpenCount--;
}

