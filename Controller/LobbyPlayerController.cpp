// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Yuwibo/Widget/PopupWidget.h"
#include "Yuwibo/Widget/LobbyBrowserWidget.h"
#include "Yuwibo/Widget/LobbyWidget.h"

ALobbyPlayerController::ALobbyPlayerController()
{
    LobbyBrowserWidgetClass = ConstructorHelpers::FClassFinder<UUserWidget>(TEXT("WidgetBlueprint'/Game/BlueprintClass/HUD/LobbyBrowserWidgetBlueprint.LobbyBrowserWidgetBlueprint_C'")).Class;
    LobbyWidgetClass = ConstructorHelpers::FClassFinder<UUserWidget>(TEXT("WidgetBlueprint'/Game/BlueprintClass/HUD/LobbyWidgetBlueprint.LobbyWidgetBlueprint_C'")).Class;
    PopupWidgetClass = ConstructorHelpers::FClassFinder<UUserWidget>(TEXT("WidgetBlueprint'/Game/BlueprintClass/HUD/PopupWidgetBlueprint.PopupWidgetBlueprint_C'")).Class;
}

void ALobbyPlayerController::BeginPlay()
{
    Super::BeginPlay();

    bShowMouseCursor = true;

    if (LobbyBrowserWidgetClass)
    {
        LobbyBrowserWidget = CreateWidget<ULobbyBrowserWidget>(GetWorld(), LobbyBrowserWidgetClass);
        if (LobbyBrowserWidget)LobbyBrowserWidget->AddToViewport();
    }

    if (LobbyWidgetClass)
    {
        LobbyWidget = CreateWidget<ULobbyWidget>(GetWorld(), LobbyWidgetClass);
        if (LobbyWidget)
        {
            LobbyWidget->AddToViewport();
            LobbyWidget->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
}


UPopupWidget* ALobbyPlayerController::GetPopup()
{
    if (!PopupWidget && PopupWidgetClass)
    {
        PopupWidget = CreateWidget<UPopupWidget>(GetWorld(), PopupWidgetClass);
        if (PopupWidget) PopupWidget->AddToViewport();
    }

    PopupWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

    return PopupWidget;
}

ULobbyWidget* ALobbyPlayerController::GetLobbyWidget()
{
    LobbyWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    return LobbyWidget;
}
