# autocoder_external

This repository contains a compiled external and examples for loading models trained for the Autocoder, an implementation of a variational autoencoder (VAE), into [Max/MSP](https://cycling74.com/). The external only runs on ARM based Macs and Max/MSP 8.2 or newer.

To install the external, download and unzip [this file](https://github.com/franzson/autocoder_external/blob/main/external/autocoder.zip), and copy autocoder.mxo into ~/Documents/Max 8/Packages/autocoder/externals and libtensorflowlite_c.dylib into /usr/local/lib.

The first time you open a patch containing the autocoder in Max, you will get an error that the file isn't secure to open. To solve this, open System Preferences->Security & Privacy and under the general tab there should be an option to allow the software to open the file. Once you do that, close Max/MSP and reopen and it should now work.

There are two example patches included, a very basic one and a second one that requires Ircam's [spat](https://forum.ircam.fr/projects/detail/spat/) to be installed.

For more details on the AI and its architecture, please see the [main autocoder repository](https://github.com/franzson/autocoder).

To train models for the autocoder, see [Link text Here](https://link-url-here.org).

If you want to compile the external by yourself, you need to first download and instal the Max/MSP SDK and make sure it is all set up and compiling, and then place the /src/autocoder directory inside ~/Documents/Max 8/max-sdk-x.x.x/source/basics, and the /build/autocoder under ~/Documents/Max 8/max-sdk-x.x.x/build/source/basics and then open autocoder.xcodeproj in XCode.
