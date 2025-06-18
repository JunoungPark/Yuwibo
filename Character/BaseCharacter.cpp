// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Yuwibo/Component/InformationComponent.h"
#include "Yuwibo/Component/MapComponent.h"
#include "Yuwibo/Component/LookComponent.h"
#include "Yuwibo/Component/HealthComponent.h"
#include "Yuwibo/Animinstance/BaseEnemyAnimInstance.h"
#include "Net/UnrealNetwork.h"
#include "Yuwibo/Component/ItemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Yuwibo/Controller/BaseAIController.h"
#include "Perception/AISense_Damage.h"
#include "Yuwibo/Controller/BasePlayerController.h"
// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharacterName = TEXT("Empty");

	GetMesh()->SetGenerateOverlapEvents(true);

	//GetMesh()->bRenderCustomDepth = true;

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	SetComponent<UInformationComponent>(Component::Information);

	SetComponent<UAnimComponent>(Component::Anim);

	SetComponent<UMapComponent>(Component::Map);

	SetComponent<ULookComponent>(Component::Look);

	SetComponent<UHealthComponent>(Component::Health);
	
	ConstructorHelpers::FClassFinder<UUserWidget> AR(TEXT("WidgetBlueprint'/Game/BlueprintClass/HUD/Alarm.Alarm_C'"));

	if (AR.Succeeded())
	{

		Alarm = CreateWidget(GetWorld(), AR.Class);

		if (Alarm)
		{

			Alarm->AddToViewport();

			Alarm->SetVisibility(ESlateVisibility::Collapsed);

		}
	}
}

// Called every frame
void ABaseCharacter::GetActorEyesViewPoint(FVector& out_Location, FRotator& out_Rotation) const
{
	//Super::GetActorEyesViewPoint(out_Location, out_Rotation);
	
	out_Location = GetMesh()->GetSocketLocation("headSocket");
	out_Rotation = GetMesh()->GetSocketRotation("headSocket")+ SocketRotation;
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	ULookComponent* Look = GetComponent<ULookComponent>(Component::Look);

	PlayerInputComponent->BindAxis(TEXT("LockUpDown"), Look, &ULookComponent::LookUpDown);
	PlayerInputComponent->BindAxis(TEXT("LockLeftRight"), Look, &ULookComponent::LookLeftRight);

	PlayerInputComponent->BindAction(TEXT("Information"), EInputEvent::IE_Pressed, GetComponent<UInformationComponent>(Component::Information), &UInformationComponent::OnOffInformation);

	auto Map = GetComponent<UMapComponent>(Component::Map);

	PlayerInputComponent->BindAction(TEXT("OnOffMap"), EInputEvent::IE_Pressed, Map, &UMapComponent::OnOffMap);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ABaseCharacter::Jump);
}

float ABaseCharacter::TakeDamage(float DamageTaken, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (auto Pawn = Cast<ABaseCharacter>(DamageCauser))
	{
		if (Pawn->TeamID == TeamID && Pawn->GetController() && Pawn->GetController()->IsA(ABaseAIController::StaticClass())) return 0.f;
	}

	UHealthComponent* Health = GetComponent<UHealthComponent>(Component::Health); 
	
	float damageApplied = Health->GetCurrentHealth();

	if (GetComponent<UEquipmentComponent>(Component::Equipment) && GetComponent<UEquipmentComponent>(Component::Equipment)->GetEquipment(ItemType::Weapon).ItemID > 0)
		DamageTaken *= (1 + Cast<UYuwiboGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetItemData(GetComponent<UEquipmentComponent>(Component::Equipment)->GetEquipment(ItemType::Boots).ItemID)->Attack);
	
	if (GetController() && GetController()->IsA(ABaseAIController::StaticClass()))
	{
		UAISense_Damage::ReportDamageEvent(
			GetWorld(),
			this,
			EventInstigator->GetPawn(),
			DamageTaken,
			DamageCauser->GetActorLocation(),
			DamageCauser->GetActorLocation());
	}

	damageApplied -= DamageTaken;

	

	Health->SetCurrentHealth(damageApplied);

	return damageApplied;
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(auto Enemy = Cast<UEnemyAnimComponent>(GetComponent<UAnimComponent>(Component::Anim)))
	SocketRotation = Enemy->GetLookRotator();
	
}

void ABaseCharacter::VIsibleAlarm_Implementation()
{
	Alarm->SetVisibility(ESlateVisibility::HitTestInvisible);
	GetWorld()->GetTimerManager().SetTimer(AlarmTimerHandle, this, &ABaseCharacter::CollapsedAlarm, 5.f, false);
}
