# OUTDATED
# YimMenu Launchpad

[YimMenu](https://github.com/YimMenu/YimMenu), a GTA V menu protecting against a wide ranges of the public crashes and improving the overall experience.

YimMenu is in my opinion the best **free** GTA V mod menu, its protections are at the level of a paid menu, you have lots of trolling options, amazing scripts from the community, etc.

## Why this project?

YimMenu does not have an official launcher, so imagine you are a YimMenu user, and like every day you log in to GTA V and inject your menu. But something strange happens, many of the functions don't work, so you go to the site, download it again and inject it and now it works. What has happened is that they have clearly released a new version of the menu and you didn't have it downloaded.

Seeing this problem, I decided to make a launchpad for the menu with [C++](https://es.wikipedia.org/wiki/C%2B%2B) and [ImGui](https://github.com/ocornut/imgui).

## How to use

In order to use the launchpad:

1. Download from the [releases page](https://github.com/ExoticGamerrrYT/YimMenu-Launchpad/releases/tag/releases).
2. Make a folder anywhere you want to and add that folder to [Windows Defender exclusion list](https://support.microsoft.com/en-us/windows/add-an-exclusion-to-windows-security-811816c0-4dfd-af4a-47e4-c301afe13b26).
3. Extract the .zip in that folder.
4. When you execute the .exe you can press `Inject` with the game open, obviously, and if something goes wrong check the console.

   > _You may want to make a shortcut to the desktop for the .exe._

   > **DO NOT DELETE ANY OF THE DLLs, IF YOU DO REPEAT FROM STEP 3**
   
   **IF YOU GET ANY WINDOWS ALERT IGNORE IT**

## Contributing

_**Please remember to contribute according to the [LICENSE](LICENSE).**_

1. Clone the repo

2. This project has been done with Curl and ImGui. ImGui is downloaded locally in the project but Curl has been downloaded via [vcpkg](https://github.com/microsoft/vcpkg), so if you use vcpkg do this:

   ```bash
   # Installing curl
   vcpkg install curl
   ```

   > Note that here vcpkg is in the environment variables and that if you use Visual Studio you have to run this `vcpkg integrate install`.

   > If you don't want to add vcpkg to the enviroment variables, in the root folder execute it with `.\vcpkg\vcpkg`.

   **BUT** if you don't use vcpkg, download the headers and libraries from their websites and add them to your Visual Studio configuration.

3. Open the `YimMenu Lauchpad.sln` file.

## License

[GNU General Public License v3.0](https://choosealicense.com/licenses/gpl-3.0/).

## Contact

Discord: `3xotic.`
