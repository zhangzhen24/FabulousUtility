// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "Framework/Text/ILayoutBlock.h"
#include "Framework/Text/SlateHyperlinkRun.h"

class FFuSlateHyperlinkRun : public FSlateHyperlinkRun
{
public:
	DECLARE_DELEGATE_OneParam(FOnHovered, const FMetadata& /*Metadata*/);
	DECLARE_DELEGATE_OneParam(FOnUnhovered, const FMetadata& /*Metadata*/);

public:
	static TSharedRef<FFuSlateHyperlinkRun> Create(const FRunInfo& InRunInfo, const TSharedRef<const FString>& InText, const FHyperlinkStyle& InStyle, FOnClick NavigateDelegate,
	                                               FOnGenerateTooltip InTooltipDelegate, FOnGetTooltipText InTooltipTextDelegate);

	static TSharedRef<FFuSlateHyperlinkRun> Create(const FRunInfo& InRunInfo, const TSharedRef<const FString>& InText, const FHyperlinkStyle& InStyle, FOnClick NavigateDelegate,
	                                               FOnGenerateTooltip InTooltipDelegate, FOnGetTooltipText InTooltipTextDelegate, const FTextRange& InRange);

	static TSharedRef<FFuSlateHyperlinkRun> Create(const FRunInfo& InRunInfo, const TSharedRef<const FString>& InText, const FHyperlinkStyle& InStyle,
	                                               FOnClick NavigateDelegate, FOnHovered InHoverdDelegate, FOnUnhovered InUnhoverdDelegate,
	                                               FOnGenerateTooltip InTooltipDelegate, FOnGetTooltipText InTooltipTextDelegate, const FTextRange& InRange);

public:
	virtual ~FFuSlateHyperlinkRun() override
	{
	}

	/** FSlateHyperlinkRun implementation */
	virtual FVector2D Measure(int32 StartIndex, int32 EndIndex, float Scale, const FRunTextContext& TextContext) const override;
	virtual TSharedRef<ILayoutBlock> CreateBlock(int32 StartIndex, int32 EndIndex, FVector2D Size, const FLayoutBlockTextContext& TextContext,
	                                             const TSharedPtr<IRunRenderer>& Renderer) override;

protected:
	void OnHoveredEvent();
	void OnUnhoveredEvent();

private:
	FFuSlateHyperlinkRun(const FRunInfo& InRunInfo, const TSharedRef<const FString>& InText, const FHyperlinkStyle& InStyle,
	                     FOnClick InNavigateDelegate,
	                     FOnGenerateTooltip InTooltipDelegate,
	                     FOnGetTooltipText InTooltipTextDelegate);

	FFuSlateHyperlinkRun(const FRunInfo& InRunInfo, const TSharedRef<const FString>& InText, const FHyperlinkStyle& InStyle,
	                     FOnClick InNavigateDelegate,
	                     FOnGenerateTooltip InTooltipDelegate,
	                     FOnGetTooltipText InTooltipTextDelegate,
	                     const FTextRange& InRange);

	FFuSlateHyperlinkRun(const FRunInfo& InRunInfo, const TSharedRef<const FString>& InText, const FHyperlinkStyle& InStyle,
	                     FOnClick InNavigateDelegate,
	                     FOnHovered InHoveredDelegate,
	                     FOnUnhovered InUnhoveredDelegate,
	                     FOnGenerateTooltip InTooltipDelegate,
	                     FOnGetTooltipText InTooltipTextDelegate,
	                     const FTextRange& InRange);


	FFuSlateHyperlinkRun(const FFuSlateHyperlinkRun& Run);

private:
	// check whether our metadata makes us a browser link
	bool CheckIsBrowserLink() const;

protected:
	FOnHovered OnHoveredDelegate;
	FOnUnhovered OnUnhoveredDelegate;

private:
	/** Whether we are an external (browser) link or not */
	bool bIsBrowserLink;

	/** The brush we use to display external links */
	const FSlateBrush* LinkBrush;
};
