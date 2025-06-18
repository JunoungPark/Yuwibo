// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ActorComponent.h"
#include "MapComponent.generated.h"

UCLASS()
class YUWIBO_API UMapWidget : public UUserWidget
{
	GENERATED_BODY()

	friend class UMapComponent;

	UPROPERTY()
	TArray<class ULocationWidget*> LocationWidget;
	UPROPERTY()
	class ABaseCharacter* Character;
	UPROPERTY(meta = (BindWidget))
	class UButton* MoveButton;
	UPROPERTY(meta = (BindWidget))
	class UImage* Map;
	UPROPERTY(meta = (BindWidget))
	class UImage* Location;
	UPROPERTY(meta = (BindWidget))
	class UImage* MiniMap;
	UPROPERTY(meta = (BindWidget))
	class UImage* MiniMapLocation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ULocationWidget> LocationWidgetClass;
	FTimerHandle CooldownTimerHandle;

	FVector2D DragPosition;

	double NewSize = 1;

private:
	UFUNCTION()
	void DragStart();

	void Drag();

	UFUNCTION()
	void DragEnd();

	void UpdateLocation();

	void OnOffMap();

	void AddLocationWidget(class AStoneStatue* Statue);
protected:

	void NativeOnInitialized()override;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)override;

	FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class YUWIBO_API UMapComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMapComponent();
private:
	UPROPERTY()
	class ABaseCharacter* Character;

	UPROPERTY()
	UMapWidget* MapWidget;

	UPROPERTY()
	class USpringArmComponent* SpringArm;

	UPROPERTY()
	class USceneCaptureComponent2D* MiniWorldSceneCapture;

	UPROPERTY()
	class UTextureRenderTarget2D* MiniWorldRenderTarget2D;

public:	

	void BeginPlay()override;

	FORCEINLINE void OnOffMap() { MapWidget->OnOffMap(); }

	void DestroyWidget();

	void AddViewportWidget();

	UFUNCTION(Client, Reliable)
	void AddLocationWidget(class AStoneStatue* Statue);

	UFUNCTION(Client, Reliable)
	void AddLocationWidgets(TArray<class AStoneStatue*> const& Statues);

	UFUNCTION(Client, Reliable)
	void RemoveLocationWidget(class AStoneStatue* Statue);


	void Bind(class ABaseCharacter* NewCharacter);

};
