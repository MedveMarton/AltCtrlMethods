// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "CtrlMethodsGameUserSettings.generated.h"

/**
 *
 */
UCLASS()
class ALTCONTROLMETHODS_API UCtrlMethodsGameUserSettings : public UGameUserSettings
{
	GENERATED_UCLASS_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetMethodIndex(int32 CtrlMethIndex);

	UFUNCTION(BlueprintPure)
	int32 GetMethodIndex() const;

	UFUNCTION(BlueprintCallable)
	static UCtrlMethodsGameUserSettings* GetCtrlMethodsGameUserSettings();

protected:
	UPROPERTY(Config)
	int32 ControlMethodIndex;
};