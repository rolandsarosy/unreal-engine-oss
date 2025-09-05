# Log On Screen with Netroles first

A simple extension of `GEEngine->AddOnScreenDebugMessage` that adds a netrole prefix to the message.

## Usage example

```
LogOnScreen(this, FString::Printf(TEXT("Achievement was found with ID '%s' and progress'%f'"), *Data.Id, Data.Progress));
```
