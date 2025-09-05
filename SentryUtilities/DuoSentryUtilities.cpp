#include "Utilities/DuoSentryUtilities.h"

#include "SentryBreadcrumb.h"
#include "SentryLibrary.h"
#include "SentrySubsystem.h"
#include "SentryDataTypes.h"

void UDuoSentryUtilities::AddSimpleBreadcrumb(FString Text, ESentryLevel Level)
{
	USentrySubsystem* SentrySubsystem = GEngine->GetEngineSubsystem<USentrySubsystem>();

	const TMap<FString, FString> Data;
	const auto Breadcrumb = USentryLibrary::CreateSentryBreadcrumb(Text, "", "Simple Breadcrumbs", Data, Level);

	SentrySubsystem->AddBreadcrumb(Breadcrumb);
}

ESentryLevel ConvertVerbosityToSentryLevel(const ELogVerbosity::Type Verbosity)
{
	switch (Verbosity)
	{
	case ELogVerbosity::Fatal:
		return ESentryLevel::Fatal;
	case ELogVerbosity::Error:
		return ESentryLevel::Error;
	case ELogVerbosity::Warning:
		return ESentryLevel::Warning;
	case ELogVerbosity::Display: // Display and Log can be treated as Info.
	case ELogVerbosity::Log:
		return ESentryLevel::Info;
	case ELogVerbosity::Verbose: // Verbose and VeryVerbose can be treated as Debug.
	case ELogVerbosity::VeryVerbose:
	default:
		return ESentryLevel::Debug;
	}
}
