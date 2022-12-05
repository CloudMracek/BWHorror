# BWHorror
A horror game I'm working on

# Building the project: Linux

Download the latest release of [spicyjpeg's `PSn00bSDK`](https://github.com/spicyjpeg/PSn00bSDK) and extract the folder to some directory you can reference later. For example:

```
mkdir ~/.sdk
cd ~/.sdk
wget https://github.com/spicyjpeg/PSn00bSDK/releases/download/v0.21/PSn00bSDK-0.21-Linux.zip
unzip PSn00bSDK-0.21-Linux.zip
rm PSn00bSDK-0.21-Linux.zip
```

This will create a directory `.sdk` in your HOME directory and extract the Linux version of PSn00bSDK there. Afterwards, the Zip archive is removed.

The build requires the environment variable `PSN00BSDK_INSTALL_DIR` to be set to the folder you extracted PSn00bSDK. For the above instructions, that would be something like this:
```
export PSN00BSDK_INSTALL_DIR=~/.sdk/PSn00bSDK-0.21-Linux
```

Put that in your `.bashrc` or the respective shell you're using.

# Building the project: Windows

Download the latest release of [spicyjpeg's `PSn00bSDK`](https://github.com/spicyjpeg/PSn00bSDK) and extract the folder to some directory you can reference later. For example:

1. Create the folder `C:\SDK`
2. Download https://github.com/spicyjpeg/PSn00bSDK/releases/download/v0.21/PSn00bSDK-0.21-Win32.zip
3. Unzip the underlying folder into `C:\SDK`
4. Remove the Zip file

This resulting folder would be `C:\SDK\PSn00bSDK-0.21-Win32`.

The build requires the environment variable `PSN00BSDK_INSTALL_DIR` to be set to the folder `C:\SDK\PSn00bSDK-0.21-Win32` in this example.
