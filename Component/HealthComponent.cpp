// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "AiComponent.h"
#include "MoveComponent.h"
#include "Yuwibo/Controller/BaseAIController.h"
#include "Yuwibo/Character/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Components/ProgressBar.h"
#include "ItemComponent.h"
#include "Yuwibo/Character/Tank.h"
#include "Yuwibo/Character/GroundTurret.h"
#include "Yuwibo/Controller/BasePlayerController.h"
#include "Yuwibo/YuwiboGameModeBase.h"

UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);

	MaxHealth = 100;
	CurrentHealth = MaxHealth;

}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, CurrentHealth); 
	DOREPLIFETIME(UHealthComponent, DissolveAmount);
	
}

void UHealthComponent::OnRep_DissolveAmount()
{
	if (Meshs.Num())
	{
		for (auto& Mesh : Meshs)
		{
			for (int32 i = 0; i < Mesh->GetNumMaterials(); i++)
			{
				if (auto DynMat = Mesh->CreateDynamicMaterialInstance(i))
				{
					DynMat->SetScalarParameterValue(FName("Process"), DissolveAmount);
				}
			}
		}
	}

	for (int32 i = 0; i < Character->GetMesh()->GetNumMaterials(); i++)
	{
		if (auto DynMat = Character->GetMesh()->CreateDynamicMaterialInstance(i))
		{
			DynMat->SetScalarParameterValue(FName("Process"), DissolveAmount);
		}
	}
}

void UHealthComponent::OnRep_CurrentHealth()
{
	OnHealthUpdate();
}

void UHealthComponent::OnHealthUpdate()
{
	if(HpWidget) HpWidget->GetPB_HpBar()->SetPercent(CurrentHealth / MaxHealth);

	if (CurrentHealth <= 0)
	{
		if (Character->GetController())
		{
			if (auto Controller = Cast<ABasePlayerController>(Character->GetController()))
			{
				Character->DisableInput(UGameplayStatics::GetPlayerController(this, 0));
				
				static_cast<AYuwiboGameModeBase*>(UGameplayStatics::GetGameMode(this))->PlayerIsEnd(Controller);
			}
			Character->GetController()->UnPossess();
		}

		if (Character->IsA(AGroundTurret::StaticClass()))
		{
			Character->GetComponent<UAnimComponent>(Component::Anim)->SetIsDead(true);
		}
		else
		{
			Character->GetMesh()->bPauseAnims = true;
			Character->GetMesh()->SetCollisionProfileName("Ragdoll");
			Character->GetMesh()->SetSimulatePhysics(true);
		}

		for (auto& Mesh : Meshs)
		{
			if(auto Skeltal = Cast<USkeletalMeshComponent>(Mesh))
			{
				Skeltal->bPauseAnims = true;
			}
			Mesh->SetCollisionProfileName("Ragdoll");
			Mesh->SetSimulatePhysics(true);
		}

		for (auto& Comp : RelatedComponents)
		{
			if (auto Ai = Cast<UAiComponent>(Comp))
			{
				Ai->RemoveCharacter(Character);
			}
			else if (auto Enemy = Cast<UEnemyComponent>(Comp))
			{
				//Ai->RemoveCharacter(Character);
			}
		}

		if (HpWidget)
		{
			HpWidget->RemoveFromParent();
			HpWidget->ConditionalBeginDestroy();
		}
		//Character->Destroy();
		//FTimerHandle DestoyTimerHandle;
		//GetWorldTimerManager().SetTimer(DestoyTimerHandle, this, &ACharacterHealth::Disappear, 8.0f);
		
		if (Character->HasAuthority()) SetComponentTickEnabled(true);
		
		
	}
}

// Sets default values for this component's properties



// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	SetComponentTickEnabled(false);

	if (Character && Character->IsLocallyControlled()) AddViewportWidget();
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!Character->HasAuthority()) return;

	if (DissolveOff)
	{
		if (DissolveAmount > 0.f)
		{
			DissolveAmount -= DeltaTime * 0.5f;

		}

		if (DissolveAmount < 0.f)
		{
			DissolveAmount = 0.f;

			DissolveOff = false;

			if (auto Move = Character->GetComponent<UMoveComponent>(Component::Move))
			{
				Move->SetCanMove(true);

				Move->SetCanAttack(true);

			}
			SetComponentTickEnabled(false);
		}

	}
	else if (DissolveAmount < 1.5f)
	{
		DissolveAmount += DeltaTime * 0.5f;

		if (DissolveAmount > 1.5f)
		{
			DissolveEnd.Broadcast();
			Character->Destroy();
		}

	}
}

void UHealthComponent::SetCurrentHealth(float healthValue)
{
	if (Character->GetLocalRole() == ROLE_Authority)
	{
		if (healthValue > CurrentHealth) CurrentHealth = FMath::Clamp(healthValue, 0.f, MaxHealth);
		else
		{
			float defense = 1;
			if (auto Equipment = Character->GetComponent<UEquipmentComponent>(Component::Equipment))
			{
				if (Equipment->GetEquipment(ItemType::Helmet).ItemID > 0)
				{
					defense -= Cast<UYuwiboGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetItemData(Equipment->GetEquipment(ItemType::Helmet).ItemID)->Defense;
				}
				if (Equipment->GetEquipment(ItemType::Armor).ItemID > 0)
				{
					defense -= Cast<UYuwiboGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetItemData(Equipment->GetEquipment(ItemType::Armor).ItemID)->Defense;
				}
				if (Equipment->GetEquipment(ItemType::Pants).ItemID > 0)
				{
					defense -= Cast<UYuwiboGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetItemData(Equipment->GetEquipment(ItemType::Pants).ItemID)->Defense;
				}
				if (Equipment->GetEquipment(ItemType::Boots).ItemID > 0)
				{
					defense -= Cast<UYuwiboGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetItemData(Equipment->GetEquipment(ItemType::Boots).ItemID)->Defense;
				}
			}
			CurrentHealth = FMath::Clamp(healthValue * defense, 0.f, MaxHealth);
		}

		OnHealthUpdate();

	}
}

void UHealthComponent::DestroyWidget()
{
	if (HpWidget)
	{
		HpWidget->RemoveFromParent();  // ������ �θ𿡼� ����
		HpWidget = nullptr;             // ���۷��� �ʱ�ȭ
	}
}

void UHealthComponent::AddViewportWidget()
{
	if (HpWidget) return; 

	if (auto UW = LoadObject<UClass>(nullptr, TEXT("WidgetBlueprint'/Game/BlueprintClass/HUD/WPB_HpBar.WPB_HpBar_C'")))
	{
		HpWidget = CreateWidget<UHpWidget>(GetWorld(), UW);

		OnHealthUpdate();

		if (HpWidget)
		{
			HpWidget->AddToViewport();
		}
	}
}

void UHealthComponent::Bind(ABaseCharacter* NewCharacter)
{
	Character = NewCharacter;
	
}

