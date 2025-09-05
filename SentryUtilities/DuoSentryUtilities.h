#pragma once

#include "CoreMinimal.h"
#include "SentryDataTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DuoSentryUtilities.generated.h"

enum class ESentryLevel : uint8;

UCLASS()
class DUOGAME_API UDuoSentryUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Sentry")
	static void AddSimpleBreadcrumb(FString Text, ESentryLevel Level = ESentryLevel::Info);
};

// Forward-declaration of the helper function.
ESentryLevel ConvertVerbosityToSentryLevel(const ELogVerbosity::Type Verbosity);

/**
 * Custom log macro that executes a standard UE_LOG and also sends the message to Sentry.
 *
 * The do-while(0) wrapper is a standard C++ pattern to ensure the macro behaves
 * like a single statement, preventing scope issues if used inside an if/else block
 * without braces.
 *
 * The '##__VA_ARGS__' syntax correctly handles cases where there are no extra format
 * arguments, preventing compiler errors from a trailing comma.
 *
 * @warning Developers must add the "SentrySubsystem" include to the top of their file, wherever this macro is used
 */
#define UE_SENTRY_LOG(CategoryName, Verbosity, Format, ...) \
    do \
    { \
        UE_LOG(CategoryName, Verbosity, Format, ##__VA_ARGS__); \
        if (GEngine) \
        { \
            if (USentrySubsystem* SentrySubsystem = GEngine->GetEngineSubsystem<USentrySubsystem>()) \
            { \
                const FString FormattedMessage = FString::Printf(Format, ##__VA_ARGS__); \
                const ESentryLevel SentryLevel = ConvertVerbosityToSentryLevel(ELogVerbosity::Verbosity); \
                SentrySubsystem->CaptureMessage(FormattedMessage, SentryLevel); \
            } \
        } \
    } while(0)
