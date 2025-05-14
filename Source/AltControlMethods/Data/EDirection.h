// DirectionEnum.h
#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EDirection : uint8
{
	Up_Left      UMETA(DisplayName = "Up-Left"),
	Down_Left    UMETA(DisplayName = "Down-Left"),
	Up_Right     UMETA(DisplayName = "Up-Right"),
	Down_Right   UMETA(DisplayName = "Down-Right"),
	Left         UMETA(DisplayName = "Left"),
	Right        UMETA(DisplayName = "Right"),
	Up           UMETA(DisplayName = "Up")
};