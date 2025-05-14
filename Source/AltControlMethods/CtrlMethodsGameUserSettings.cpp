// Fill out your copyright notice in the Description page of Project Settings.


#include "CtrlMethodsGameUserSettings.h"

UCtrlMethodsGameUserSettings::UCtrlMethodsGameUserSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	ControlMethodIndex = 0;
}


void UCtrlMethodsGameUserSettings::SetMethodIndex(int32 CtrlMethIndex)
{
	ControlMethodIndex = CtrlMethIndex;
}

int32 UCtrlMethodsGameUserSettings::GetMethodIndex() const
{
	return ControlMethodIndex;
}

UCtrlMethodsGameUserSettings* UCtrlMethodsGameUserSettings::GetCtrlMethodsGameUserSettings()
{
	return Cast<UCtrlMethodsGameUserSettings>(UGameUserSettings::GetGameUserSettings());
}