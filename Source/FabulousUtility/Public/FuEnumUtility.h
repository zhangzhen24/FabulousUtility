// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

namespace FuEnumUtility
{
	template <typename EnumType>
	DLLEXPORT int32 GetIndexByValue(const EnumType Value)
	{
		return StaticEnum<EnumType>()->GetIndexByValue(static_cast<int64>(Value));
	}

	template <typename EnumType>
	DLLEXPORT FString GetNameStringByValue(const EnumType Value)
	{
		return StaticEnum<EnumType>()->GetNameStringByValue(static_cast<int64>(Value));
	}
}
