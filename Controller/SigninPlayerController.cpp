// Fill out your copyright notice in the Description page of Project Settings.


#include "SigninPlayerController.h"
#include "Yuwibo/Widget/SigninPageWidget.h"
#include "Blueprint/UserWidget.h"
#include "Yuwibo/Widget/PopupWidget.h"
#include "Yuwibo/Networking/NetworkingSubsystem.h"
ASigninPlayerController::ASigninPlayerController()
{
    SigninWidgetClass = ConstructorHelpers::FClassFinder<UUserWidget>(TEXT("WidgetBlueprint'/Game/BlueprintClass/HUD/SigninPageWidgetBlueprint.SigninPageWidgetBlueprint_C'")).Class;
    UserNameWidgetClass = ConstructorHelpers::FClassFinder<UUserWidget>(TEXT("WidgetBlueprint'/Game/BlueprintClass/HUD/UserNameWidgetBlueprint.UserNameWidgetBlueprint_C'")).Class;
    PopupWidgetClass = ConstructorHelpers::FClassFinder<UUserWidget>(TEXT("WidgetBlueprint'/Game/BlueprintClass/HUD/PopupWidgetBlueprint.PopupWidgetBlueprint_C'")).Class;
}

void ASigninPlayerController::BeginPlay()
{
    Super::BeginPlay();

    bShowMouseCursor = true;

    if (SigninWidgetClass)
    {
        GetGameInstance()->GetSubsystem<UNetworkingSubsystem>()->Connect(FString("192.168.106.128"), 7777);

        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), SigninWidgetClass);
        if (CurrentWidget) CurrentWidget->AddToViewport();
    }
}

void ASigninPlayerController::SignupModeChange()
{
    if (auto SigninPageWidget = Cast<USigninPageWidget>(CurrentWidget)) SigninPageWidget->ModeChange();
}

void ASigninPlayerController::ShowUserNameWidget()
{
    if (CurrentWidget) CurrentWidget->RemoveFromParent();
    if (UserNameWidgetClass)
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), UserNameWidgetClass);
        if (CurrentWidget) CurrentWidget->AddToViewport();
    }
}

UPopupWidget* ASigninPlayerController::GetPopup()
{
    if (!PopupWidget && PopupWidgetClass)
    {
        PopupWidget = CreateWidget<UPopupWidget>(GetWorld(), PopupWidgetClass);
        if (PopupWidget) PopupWidget->AddToViewport();
    }

    PopupWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

    return PopupWidget;
}