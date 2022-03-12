# autocoder_external

This repository contains a compiled external and examples for loading models trained for the Autocoder, an implementation of a variational autoencoder (VAE), into [Max/MSP](https://cycling74.com/). The external only runs on Max/MSP 8.2 or newer.

To install the external, download and unzip [this file](https://github.com/franzson/autocoder_external/blob/main/external/autocoder-universal.zip), and copy autocoder.mxo into ~/Documents/Max 8/Packages/autocoder/externals and libtensorflowlite_c.dylib into /usr/local/lib. If the directory doesn't exist then you can create it by opening the terminal and writing <b>sudo mkdir /usr/local/lib<b>. You might need to enter your password.

The first time you initialize the autocoder or open a patch containing the autocoder in Max, you will get an error that the file isn't secure to open. To solve this, open System Preferences->Security & Privacy and under the general tab there should be an option to allow the software to open the file. Once you do that, close Max/MSP and reopen and it should now work.

There are two example patches included, a very basic one and a second one that requires Ircam's [spat](https://forum.ircam.fr/projects/detail/spat/) to be installed.

For more details on the AI and its architecture, please see the [main autocoder repository](https://github.com/franzson/autocoder).

You can download an example model [here](https://github.com/franzson/autocoder_models).

To train models for the autocoder based on a single file, see [here](https://github.com/franzson/autocoder_training).

To train a model based on a corpus of sounds you will need to refer to the [python implementation of the autocoder](https://github.com/franzson/autocoder).

If the models sound sinusoidal or have a lot of artifacts, try lowering the whitening value. If they are boom-y or have resonant peaks, try increasing the brightness. Different models sound better/worse at different generation intervals, you can adjust the range between latent space updates in the time section. A small scooping of the middle frequencies in the filter section is often helpful to produce a more natural sound.

If you want to compile the external by yourself, you need to first download and instal the Max/MSP SDK and make sure it is all set up and compiling and then place the files in the /src/autocoder directory inside ~/Documents/Max 8/max-sdk-x.x.x/source/basics/autocoder, and the files in /build/autocoder under ~/Documents/Max 8/max-sdk-x.x.x/build/source/basics/autocoder and then open autocoder.xcodeproj in XCode.
