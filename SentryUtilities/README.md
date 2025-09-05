# Useful logging utilities for [Sentry](https://sentry.io)
A short set of utilities for those that use Sentry for their application health monitoring and bug tracking. Simple breadcrumbs and drop-in macro replacement for UE_LOG that sends Sentry logs as well.

## Contents
- `UE_LOG` replacement macro `UE_SENTRY_LOG`, which is a macro wrapper to a macro that will also log a Sentry message.
- A simpler way to log Sentry `breadcrumbs` with `AddSimpleBreadcrumb` using blueprints or C++

## Examples:

#### Simple Breadcrumbs
![image](https://github.com/rolandsarosy/unreal-engine-oss/blob/main/SentryUtilities/img_sentry_extensions_breadcrumbs.png?raw=true)

#### Sentry macro wrapper
![image](https://github.com/rolandsarosy/unreal-engine-oss/blob/main/SentryUtilities/img_sentry_extensions_code.png?raw=true)

## Dependencies
- None.
