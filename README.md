# MySimsModLoader

> [!IMPORTANT]
> This software is in a pre-release state.
> Your game **will** crash, and it will **not** tell you why.
> Back-up your saves!

The MySims modloader is a new way to install and distribute mods. Instead of modifying game files, or even worse sharing installers. You can now simply download a mod and put it in your mods folder.

For any questions visit the [MySims Clubhouse](https://discord.gg/2j9mU6zNJH) modding Discord channel.

## Supported versions

|Version|Supported|
|-|-|
|MySims Release / Origin (EA App)|✔️|
|MySims Taco Bell Edition|✔️|
|MySims EA App / Steam|When it releases|
|MySims Kingdom EA App / Steam|When it releases|

## Installation

1. Go to the latest [Release](https://github.com/ThuverX/MySimsModLoader/releases)
2. Download the file `windows-x86`
3. Unzip it into your game folder, next to `MySims.exe` (`MySims/bin/`)
    - In the end you should have `mods`, `dsound.dll` and `MySims.exe` in the same folder.
4. Done! Launch the game like normal.

## Uninstallation
1. Remove `dsound.dll`
2. Optionally remove the `mods` folder

## Installing mods

- Once you have found a mod that you like, unzip its folder into the `mods` folder.
    - If it looks like `mods/MOD NAME/mod.xml` you've done it correctly!.

To uninstall simply remove that folder from the `mods`.

## Making mods

Making mods is currently limited to texture and lua based mods. For now take a look at these places:
- ⚠️ WORK IN PROGRESS [Wiki](https://github.com/ThuverX/MySimsModLoader/wiki)
- ⚠️ UNRELEASED [MySims Editor](https://github.com/ThuverX/MySimsEditor) to modify and create game files like worlds and models.
- [Examples](examples).

## Building  
1. **Clone the repository**:  
   ```sh
   git clone --recurse-submodules https://github.com/ThuverX/MySimsModLoader.git
   cd MySimsModLoader
   ```  
2. **Generate the build files using CMake**:
   ```sh
   cmake -B build -G "Visual Studio 17 2022" -A Win32
   ```  
   > ⚠️ **Make sure to specify `-A Win32`** to build in **x86 mode**!  

3. **Build the project**:  
   ```sh
   cmake --build build --config Release
   ```  

## Acknowledgements

- This project could never have been done without the awesome work done for the [Spore ModAPI](https://github.com/emd4600/Spore-ModAPI).
- Honorary mention to [MorcuTool](https://github.com/Olivercomet/MorcuTool) for getting me interested in making this modloader.

## Contributors
This project was made possible by:
- [ThuverX](https://github.com/ThuverX)
- [bottledlactose](https://github.com/bottledlactose)
- [MikeyLORR](https://github.com/MikeyLORR)
- [Lyralei1](https://github.com/Lyralei1)
- [asteriddle](https://github.com/asteriddle)
- [Click here for an updated list of contributors](https://github.com/ThuverX/MySimsModLoader/graphs/contributors)
