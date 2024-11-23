// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/RichTextBlockDecorator.h"
#include "Framework/Text/TextDecorators.h"
#include "FuHyperlinkDecorator.generated.h"


USTRUCT(Blueprintable, BlueprintType)
struct FABULOUSUI_API FRichHyperlinkStyleRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = Appearance)
	FHyperlinkStyle HlStyle;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHyperlinkClicked, const FString&, Action, const FString&, Url);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHyperlinkHovered, const FString&, Url);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHyperlinkUnhovered, const FString&, Url);

/**
 * Hyperlink Decorator
 */
UCLASS()
class FABULOUSUI_API UFuHyperlinkDecorator : public URichTextBlockDecorator
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnHyperlinkClicked OnHyperlinkClicked;

	UPROPERTY(BlueprintAssignable)
	FOnHyperlinkHovered OnHyperlinkHovered;

	UPROPERTY(BlueprintAssignable)
	FOnHyperlinkUnhovered OnHyperlinkUnhovered;

public:
	UFuHyperlinkDecorator(const FObjectInitializer& ObjectInitializer);

	virtual TSharedPtr<ITextDecorator> CreateDecorator(URichTextBlock* InOwner) override;
	const FHyperlinkStyle* FindHyperlinkStyle(FName RowName, bool bWarnIfMissing) const;
	const FRichHyperlinkStyleRow* FindHyperlinkRow(FName RowName, bool bWarnIfMissing) const;

protected:
	UPROPERTY(EditAnywhere, Category = Appearance)
	FHyperlinkStyle DefaultHyperlinkStyle;

	UPROPERTY(EditAnywhere, Category = Appearance, meta = (RequiredAssetDataTags = "RowStructure=/Script/FabulousUI.RichHyperlinkRow"))
	class UDataTable* HyperlinkSet;
};
