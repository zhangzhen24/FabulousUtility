// Fill out your copyright notice in the Description page of Project Settings.


#include "RichTextBlock/Hyperlink/FuHyperlinkDecorator.h"
#include "RichTextBlock/Hyperlink/FuSlateHyperlinkRun.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuHyperlinkDecorator)


class FFuHyperlinkDecorator : public FHyperlinkDecorator
{
public:
	FFuHyperlinkDecorator(FString InId,
	                      const FSlateHyperlinkRun::FOnClick& InNavigateDelegate,
	                      const FFuSlateHyperlinkRun::FOnHovered& InHoveredDelegate,
	                      const FFuSlateHyperlinkRun::FOnUnhovered& InUnhoveredDelegate,
	                      const FSlateHyperlinkRun::FOnGetTooltipText& InToolTipTextDelegate,
	                      const FSlateHyperlinkRun::FOnGenerateTooltip& InToolTipDelegate,
	                      const FHyperlinkStyle& InDefaultHyperlinkStyle)
		: FHyperlinkDecorator(InId, InNavigateDelegate, InToolTipTextDelegate, InToolTipDelegate)
		  , DefaultHyperlinkStyle(InDefaultHyperlinkStyle)
		  , HoveredDelegate(InHoveredDelegate)
		  , UnhoveredDelegate(InUnhoveredDelegate)

	{
	}


	static TSharedRef<FFuHyperlinkDecorator> Create(FString InId,
	                                                const FSlateHyperlinkRun::FOnClick& InNavigateDelegate,
	                                                const FFuSlateHyperlinkRun::FOnHovered& InHoveredDelegate,
	                                                const FFuSlateHyperlinkRun::FOnUnhovered& InUnhoveredDelegate,
	                                                const FSlateHyperlinkRun::FOnGetTooltipText& InToolTipTextDelegate,
	                                                const FSlateHyperlinkRun::FOnGenerateTooltip& InToolTipDelegate,
	                                                const FHyperlinkStyle& InDefaultHyperlinkStyle)
	{
		return MakeShareable(new FFuHyperlinkDecorator(InId, InNavigateDelegate, InHoveredDelegate, InUnhoveredDelegate,
		                                               InToolTipTextDelegate, InToolTipDelegate,
		                                               InDefaultHyperlinkStyle));
	}

	virtual TSharedRef<ISlateRun> Create(const TSharedRef<class FTextLayout>& TextLayout,
	                                     const FTextRunParseResults& RunParseResult,
	                                     const FString& OriginalText,
	                                     const TSharedRef<FString>& InOutModelText,
	                                     const ISlateStyle* Style) override
	{
		FString StyleName = TEXT("style");

		// 查找style样式名称
		const FTextRange* const MetaDataStyleNameRange = RunParseResult.MetaData.Find(TEXT("style"));
		if (MetaDataStyleNameRange != NULL)
		{
			const FString MetaDataStyleName = OriginalText.Mid(MetaDataStyleNameRange->BeginIndex, MetaDataStyleNameRange->EndIndex - MetaDataStyleNameRange->BeginIndex);
			StyleName = *MetaDataStyleName;
		}

		bool bHasTextStyle = false;

		//查找TextStyle
		FTextBlockStyle TextStyle;
		if (Style->HasWidgetStyle<FTextBlockStyle>(FName(*StyleName)))
		{
			bHasTextStyle = true;
			TextStyle = Style->GetWidgetStyle<FTextBlockStyle>(FName(*StyleName));
		}

		//记录文字范围
		FTextRange ModelRange;
		ModelRange.BeginIndex = InOutModelText->Len();
		*InOutModelText += OriginalText.Mid(RunParseResult.ContentRange.BeginIndex, RunParseResult.ContentRange.EndIndex - RunParseResult.ContentRange.BeginIndex);
		ModelRange.EndIndex = InOutModelText->Len();

		FRunInfo RunInfo(RunParseResult.Name);
		for (const TPair<FString, FTextRange>& Pair : RunParseResult.MetaData)
		{
			RunInfo.MetaData.Add(Pair.Key, OriginalText.Mid(Pair.Value.BeginIndex, Pair.Value.EndIndex - Pair.Value.BeginIndex));
		}


		//获取HyperlinkStyle
		FHyperlinkStyle HyperlinkStyle;
		if (IsValid(Decorator))
		{
			if (const FHyperlinkStyle* StylePtr = Decorator->FindHyperlinkStyle(FName(*StyleName), false))
			{
				HyperlinkStyle = *StylePtr;
			}
			else
			{
				HyperlinkStyle = DefaultHyperlinkStyle;
			}
		}
		else
		{
			if (Style->HasWidgetStyle<FHyperlinkStyle>(FName(*StyleName)))
			{
				HyperlinkStyle = Style->GetWidgetStyle<FHyperlinkStyle>(FName(*StyleName));
			}
			else
			{
				HyperlinkStyle = DefaultHyperlinkStyle;
			}
		}


		//如果有TextStyle则使用TextStyle的字体样式
		if (bHasTextStyle)
		{
			HyperlinkStyle.TextStyle = TextStyle;
		}

		return FFuSlateHyperlinkRun::Create(RunInfo, InOutModelText, HyperlinkStyle, NavigateDelegate, HoveredDelegate, UnhoveredDelegate, ToolTipDelegate, ToolTipTextDelegate,
		                                    ModelRange);
	}

	virtual bool Supports(const FTextRunParseResults& RunInfo, const FString& Text) const override
	{
		if (RunInfo.Name == ParseName && RunInfo.MetaData.Contains(TEXT("href")))
		{
			return true;
		}
		return false;
	}

protected:
	UFuHyperlinkDecorator* Decorator;
	FHyperlinkStyle DefaultHyperlinkStyle;

	//ParseName !!!
	FString ParseName = FString("a");

	FFuSlateHyperlinkRun::FOnHovered HoveredDelegate;
	FFuSlateHyperlinkRun::FOnUnhovered UnhoveredDelegate;
};


/////////////////////////////////////////////////////////////////////////////////////////////
///FuHyperlinkDecorator
UFuHyperlinkDecorator::UFuHyperlinkDecorator(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TSharedPtr<ITextDecorator> UFuHyperlinkDecorator::CreateDecorator(URichTextBlock* InOwner)
{
	return FFuHyperlinkDecorator::Create(TEXT(""),
	                                     FFuSlateHyperlinkRun::FOnClick::CreateLambda([&](const FSlateHyperlinkRun::FMetadata& Metadata)
		                                     {
			                                     FString Action = TEXT("");
			                                     if (Metadata.Contains(TEXT("action")))
			                                     {
				                                     Action = *Metadata.Find(TEXT("action"));
			                                     }

			                                     FString Url = TEXT("");
			                                     if (Metadata.Contains(TEXT("url")))
			                                     {
				                                     Url = *Metadata.Find(TEXT("url"));
			                                     }
			                                     OnHyperlinkClicked.Broadcast(Action, Url);
		                                     }
	                                     )
	                                     ,
	                                     FFuSlateHyperlinkRun::FOnHovered::CreateLambda([&](const FSlateHyperlinkRun::FMetadata& Metadata)
	                                     {
		                                     FString Url = TEXT("");
		                                     if (Metadata.Contains(TEXT("url")))
		                                     {
			                                     Url = *Metadata.Find(TEXT("url"));
		                                     }
		                                     OnHyperlinkHovered.Broadcast(Url);
	                                     }),
	                                     FFuSlateHyperlinkRun::FOnUnhovered::CreateLambda([&](const FSlateHyperlinkRun::FMetadata& Metadata)
	                                     {
		                                     FString Url = TEXT("");
		                                     if (Metadata.Contains(TEXT("url")))
		                                     {
			                                     Url = *Metadata.Find(TEXT("url"));
		                                     }
		                                     OnHyperlinkUnhovered.Broadcast(Url);
	                                     }),
	                                     FFuSlateHyperlinkRun::FOnGetTooltipText(),
	                                     FFuSlateHyperlinkRun::FOnGenerateTooltip(),
	                                     DefaultHyperlinkStyle
	);
}


const FHyperlinkStyle* UFuHyperlinkDecorator::FindHyperlinkStyle(FName RowName, bool bWarnIfMissing) const
{
	const FRichHyperlinkStyleRow* HlRow = FindHyperlinkRow(RowName, bWarnIfMissing);
	if (HlRow)
	{
		return &HlRow->HlStyle;
	}
	return &DefaultHyperlinkStyle;
}

const FRichHyperlinkStyleRow* UFuHyperlinkDecorator::FindHyperlinkRow(FName RowName, bool bWarningIfMissing) const
{
	if (HyperlinkSet)
	{
		FString ContextString;
		auto Row = HyperlinkSet->FindRow<FRichHyperlinkStyleRow>(RowName, ContextString);
		return Row;
	}

	return nullptr;
}
