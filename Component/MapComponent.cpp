// Fill out your copyright notice in the Description page of Project Settings.


#include "MapComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Yuwibo/Widget/LocationWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "PaperSprite.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
//#include "C:\Program Files\Epic Games\UE_5.1\Engine\Plugins\2D\Paper2D\Source\Paper2D\Classes\PaperSpriteComponent.h"
#include "Yuwibo/Actor/StoneStatue.h"
#include "UObject/UObjectGlobals.h"
#include "GameFramework/Character.h"
#include "Yuwibo/Character/BaseCharacter.h"

void UMapWidget::DragStart()
{
	DragPosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this) - MoveButton->GetRenderTransform().Translation;

	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UMapWidget::Drag, 0.01f, true);
}

void UMapWidget::Drag()
{
	if (auto CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(MoveButton))
	{
		MoveButton->SetRenderTranslation(UWidgetLayoutLibrary::GetMousePositionOnViewport(this) - DragPosition);
		UpdateLocation();
	}
}

void UMapWidget::DragEnd()
{
	GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);

}

void UMapWidget::UpdateLocation()
{
	auto CharacterLocation = Character->GetActorLocation();
	auto X = CharacterLocation.X - 200000;
	auto Y = CharacterLocation.Y + 200000;
	
	X /= 200000;
	//X /= 4000;
	X *= 540 * NewSize;
	//
	Y /= 200000;
	//Y /= 4000;
	Y *= 540 * NewSize;

	auto Translation = MoveButton->GetRenderTransform().Translation;
	Location->SetRenderTranslation(Translation + FVector2D(X, Y));
	Location->SetRenderTransformAngle(Character->GetRootComponent()->GetComponentRotation().Yaw + 135.);

	for (auto Widget : LocationWidget)
	{
		Widget->SetMapLocation(Translation, NewSize);
	}

}

void UMapWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	MoveButton->OnPressed.AddDynamic(this, &UMapWidget::DragStart);
	MoveButton->OnReleased.AddDynamic(this, &UMapWidget::DragEnd);
}

void UMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (Character && !Character->IsLocallyControlled()) return;

	if(MoveButton->GetVisibility() == ESlateVisibility::Visible)
		UpdateLocation();
	else
	{ 
		if(MiniMapLocation) MiniMapLocation->SetRenderTransformAngle(Character->GetRootComponent()->GetComponentRotation().Yaw + 135.);

		auto Vector = FVector2D(Character->GetRootComponent()->GetComponentLocation().X, Character->GetRootComponent()->GetComponentLocation().Y);
		for (auto Widget : LocationWidget)
		{
			Widget->SetMapLocation(Vector);
		}
	}
	
}

FReply UMapWidget::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	auto Delta = InMouseEvent.GetWheelDelta();
	if (Delta != 0)
	{

		NewSize = FMath::Clamp(NewSize += 0.2 * Delta, 1, 6);
		Map->SetDesiredSizeOverride(FVector2D(1080. * NewSize));

	}
	return Super::NativeOnMouseWheel(InGeometry, InMouseEvent);

}

void UMapWidget::OnOffMap()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (MoveButton->GetVisibility() == ESlateVisibility::Collapsed)
	{
		NewSize = 1;
		Map->SetDesiredSizeOverride(FVector2D(1080. * NewSize));

		MoveButton->SetRenderTranslation(FVector2D::ZeroVector);

		PlayerController->SetShowMouseCursor(true);

		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, MoveButton, EMouseLockMode::DoNotLock);
		
		MiniMapLocation->SetVisibility(ESlateVisibility::Collapsed);
		
		MiniMap->SetVisibility(ESlateVisibility::Collapsed);

		MoveButton->SetVisibility(ESlateVisibility::Visible);

		UpdateLocation();

		Location->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		for (auto Widget : LocationWidget)
		{
			Widget->SetMiniMapMode(false);
		}
	}
	else
	{
		PlayerController->SetShowMouseCursor(false);

		UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);

		Location->SetVisibility(ESlateVisibility::Collapsed);

		MoveButton->SetVisibility(ESlateVisibility::Collapsed);

		MiniMap->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		MiniMapLocation->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		for (auto Widget : LocationWidget)
		{
			Widget->SetMiniMapMode(true);
		}
	}
}

void UMapWidget::AddLocationWidget(AStoneStatue* Statue)
{
	if (auto Widget = CreateWidget<ULocationWidget>(GetWorld(), LocationWidgetClass))
	{
		Widget->SetActor(Statue);
		
		if (MoveButton->GetVisibility() == ESlateVisibility::Collapsed)
		{
			Widget->MiniMapLocation->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			Widget->MapLocation->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		LocationWidget.Emplace(Widget);

		UpdateLocation();
		
		Widget->AddToViewport();

	}
}

// Sets default values for this component's properties
UMapComponent::UMapComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	//ConstructorHelpers::FClassFinder<UMapWidget> MW(TEXT("WidgetBlueprint'/Game/BlueprintClass/HUD/MapWidgetBlueprint.MapWidgetBlueprint_C'"));
	//if (MW.Succeeded())
	//{
	//	if (auto Widget = CreateWidget(GetWorld(), MW.Class))
	//	{
	//		MapWidget = static_cast<UMapWidget*>(Widget);
	//
	//		MapWidget->bIsFocusable = true;
	//		if (MapWidget->Character == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)) MapWidget->AddToViewport();
	//	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetArmLength = 1000.;
	SpringArm->SetWorldRotation(FRotator(-90., 0., -90.));
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;

	MiniWorldSceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>("MiniWorldSceneCapture");
	MiniWorldSceneCapture->SetupAttachment(SpringArm);
	MiniWorldSceneCapture->ProjectionType = ECameraProjectionMode::Orthographic;
	MiniWorldSceneCapture->CaptureSource = ESceneCaptureSource::SCS_SceneColorSceneDepth;
	MiniWorldSceneCapture->OrthoWidth = 5000;

	//if(MapWidget)
	//{
	//	UTexture* Texture = static_cast<UTexture*>(MiniWorldRenderTarget2D);
	//	UTexture2D* Texture2D = static_cast<UTexture2D*>(Texture);
	//
	//	MapWidget->MiniMap->SetBrushFromTexture(Texture2D);
	//}

	TArray<AActor*> AllActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActor);
	
	for (auto Actor : AllActor)
	{
		if (Cast<ACharacter>(Actor)) MiniWorldSceneCapture->HiddenActors.Emplace(Actor);
	}
		
	//}
	// ...
}

void UMapComponent::BeginPlay()
{
	Super::BeginPlay();

	MiniWorldRenderTarget2D = NewObject<UTextureRenderTarget2D>();
	MiniWorldRenderTarget2D->InitAutoFormat(256, 256);

	MiniWorldSceneCapture->TextureTarget = MiniWorldRenderTarget2D;

	if (Character->IsLocallyControlled()) AddViewportWidget();
}

void UMapComponent::DestroyWidget()
{
	if (MapWidget)
	{
		
		if (MapWidget->LocationWidget.Num())
		{
			for (int i = 0; i < MapWidget->LocationWidget.Num(); i++)
			{
				if (MapWidget->LocationWidget[i])
				{
					MapWidget->LocationWidget[i]->RemoveFromParent();
					MapWidget->LocationWidget[i] = nullptr;
				}
			}
			
			MapWidget->LocationWidget.Empty();
		}

		MapWidget->RemoveFromParent();  // ������ �θ𿡼� ����
		MapWidget = nullptr;             // ���۷��� �ʱ�ȭ
	}
}

void UMapComponent::AddViewportWidget()
{
	if (MapWidget) return;

	if (auto MW = LoadObject<UClass>(nullptr, TEXT("WidgetBlueprint'/Game/BlueprintClass/HUD/MapWidgetBlueprint.MapWidgetBlueprint_C'")))
	{
		if (auto Widget = CreateWidget(GetWorld(), MW))
		{
			MapWidget = static_cast<UMapWidget*>(Widget);
			if (MapWidget)
			{
				MapWidget->Character = Character;

				if(MiniWorldRenderTarget2D)
				{
					UTexture* Texture = static_cast<UTexture*>(MiniWorldRenderTarget2D);
					UTexture2D* Texture2D = static_cast<UTexture2D*>(Texture);

					MapWidget->MiniMap->SetBrushFromTexture(Texture2D);
				}

				MapWidget->SetIsFocusable(true);

				MapWidget->AddToViewport();
			}
		}
	}
}

void UMapComponent::AddLocationWidget_Implementation(AStoneStatue* Statue)
{
	if (MapWidget) MapWidget->AddLocationWidget(Statue);
};

void UMapComponent::AddLocationWidgets_Implementation(TArray<class AStoneStatue*> const& Statues)
{
	if (!MapWidget) return;

	for (auto& Statue : Statues)
	{
		MapWidget->AddLocationWidget(Statue);
	}
};

void UMapComponent::RemoveLocationWidget_Implementation(AStoneStatue* Statue)
{
	if (!MapWidget) return;

	for (auto Widget : MapWidget->LocationWidget)
	{
		if (Statue == Widget->GetActor())
		{
			MapWidget->LocationWidget.Remove(Widget);
			Widget->RemoveFromParent();

			return;
		}
	}
}

void UMapComponent::Bind(ABaseCharacter* NewCharacter)
{
	SpringArm->SetupAttachment(NewCharacter->GetRootComponent());

	Character = NewCharacter;

	MiniWorldSceneCapture->HiddenActors.Emplace(Character);
}
