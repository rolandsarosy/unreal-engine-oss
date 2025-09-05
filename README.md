# Unreal Engine Open Source Collection
A collection of open-source **code snippets, modules, and utilities** I made for for Unreal Engine over the years.

## Usage
My personal opinion is that individual **code snippets are more useful than packaged libraries**. You will **need to change the naming scheme of the symbols** for to suit your project's slug.

## Content
Here is a brief overview of the contents of each folder. For a more detailed explanation, please see the README.md file inside each folder.

---
#### [SIMPLE ACTIONS](SimpleActions)
A simplified, replicated GAS-like ability system. Incredibly lightweight and backed by gameplay tags. Supports Actions (one-shots) and effects (over-time) as well.

---
#### [GAMEPLAY IDENTITY](GameplayIdentity)
A simple, replicated, Doom-inspired keycard-based, identity system that is backed by gameplay tags. Use it for lockable doors, chests as such. Identities can be assigned, removed, listened for.

---
#### [PIE EXTENSIONS](PIEExtensions)
An editor-only module for those working on co-op games. Creates additional buttons which start the PIE session with different window configs, player counts, netmodes. All my homies hate dropdown menus.

---
#### [MUSIC MANAGER](MusicManager)
An in-world actor to act as a replicated, synchronized music player for multiplayer games. Supports _licensed track skipping_, looping at specific points after track end, custom fadeins, fadeouts, etc..

---
#### [SENTRY UTILITIES](SentryUtilities)
A short set of utilities for those that use [Sentry](https://sentry.io/) for their application health monitoring and bug tracking. Simple breadcrumbs and drop-in macro replacement for UE_LOG that sends Sentry logs as well.

---
#### [LOGGING](Logging)
On-screen log function with a C++, replication focus. Prints out whether the code is running on clients or on the server.

---
## Getting Started
To use any of the code in this repository, simply clone or download the repository, or copy the files you need into your Unreal Engine project. Each folder contains a more detailed README.md with specific instructions.

## Contributing
Contributions are welcome! If you have any improvements or new additions, feel free to create a pull request. Please make sure to follow the existing code style and to update the relevant README.md files.

## Questions and how-to
Did I miss something, or not sure how to use the code I provided?
You can reach out for questions by:
- E-mail: `hello@rolandsarosy.com`
- Opening a ticket :) 

## License
This project is licensed under the MIT License.
