// Fill out your copyright notice in the Description page of Project Settings.


#include "GunDefaultCameraShakeBase.h"
#include "Shakes/PerlinNoiseCameraShakePattern.h"
UGunDefaultCameraShakeBase::UGunDefaultCameraShakeBase(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
	UPerlinNoiseCameraShakePattern* ShakePattern = Cast<UPerlinNoiseCameraShakePattern>(GetRootShakePattern());
	ShakePattern->LocationAmplitudeMultiplier = 0.f;
	ShakePattern->LocationFrequencyMultiplier = 0.f;
	ShakePattern->RotationAmplitudeMultiplier = 1.2f;
	ShakePattern->RotationFrequencyMultiplier = 5.f;
	ShakePattern->Roll.Amplitude = 0.f;
	ShakePattern->Duration = 0.f;
	ShakePattern->BlendInTime = 0.f;
	ShakePattern->BlendOutTime = 0.f;
}
