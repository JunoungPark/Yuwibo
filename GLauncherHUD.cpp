// Fill out your copyright notice in the Description page of Project Settings.


#include "GLauncherHUD.h"
#include "Engine/Canvas.h"

void AGLauncherHUD::DrawHUD()
{
	Super::DrawHUD(); 
    
    UTexture2D* CrosshairTexture = LoadObject<UTexture2D>(NULL, (TEXT("Texture2D'/Game/BlueprintClass/HUD/GLHUD.GLHUD'")), NULL, LOAD_None, NULL);
    if (CrosshairTexture)
    {
        // ĵ���� �߽��� ã���ϴ�.
        FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
        double Size = 25.;
        // �ؽ�ó �߽��� ĵ���� �߽ɿ� �µ��� �ؽ�ó�� ũ�� ���� ��ŭ �������� �ݴϴ�.
        FVector2D CrossHairDrawPosition(Center.X - (Size * 0.5f), Center.Y - (Size * 0.5f));

        // �߽����� ���ؼ��� �׸��ϴ�.
        FCanvasTileItem TileItem(CrossHairDrawPosition, CrosshairTexture->GetResource(), FVector2D(Size), FLinearColor::White);
        TileItem.BlendMode = SE_BLEND_Translucent;
        Canvas->DrawItem(TileItem);
    }
}
