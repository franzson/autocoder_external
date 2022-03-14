# AUTOCODER FOR MAX/MSP

This repository contains a compiled external and examples for loading models trained for the Autocoder, an implementation of a variational autoencoder (VAE), into [Max/MSP](https://cycling74.com/).

The external only runs on Max/MSP 8.2 or newer on a Mac.

For more details on the AI and its architecture, please see [here](https://github.com/franzson/autocoder).
 
# INSTALLATION

Make sure to install, run and exit Max before continuing, otherwise some target directories might be missing. 

To install the external, download and unzip [this file](https://github.com/franzson/autocoder_external/raw/main/external/autocoder.zip).

Open terminal and navigate to where the file was unzipped:
<pre>cd ~/Downloads/autocoder</pre>

First run: 
<pre>chmod +x ./install.sh</pre>

then run:
<pre>./install.sh PATH_TO_MAX_EXTERNALS
e.g. ./install.sh "/Users/username/Documents/Max 8"</pre>

The install script will ask you for your password, attempt to copy the autocoder.mxo external into Max/MSP external pads, then create the directory /usr/local/lib if it doesn't already exist, and attempt to copy the file libtensorflowlite_c.dylib there.

The first time you initialize the autocoder, or open a patch containing it in Max, you will get an error that the file isn't secure to open:

<img src="https://github.com/franzson/autocoder_external/raw/main/img/warning-1.jpg" width="200">

When you press ok, a second warning pops up:

<img src="https://github.com/franzson/autocoder_external/raw/main/img/warning-2.jpg" width="200">

To solve this, press cancel and then open System Preferences->Security & Privacy.
Under the general tab there should be an option to allow the software to open the file:

<img src="https://github.com/franzson/autocoder_external/raw/main/img/warning-3.jpg" width="400">

Once you do that, close Max/MSP and reopen it and instansiate the autocoder external (you can do this by loading one of the example patches or by pressing 'n' to add a new object to an empty patch and writing 'autocoder' followed by enter). This will throw this warning:

<img src="https://github.com/franzson/autocoder_external/raw/main/img/warning-4.jpg" width="200">

you can just hit open and it should now work.


If for some reason things don't seem to be working, download the package again, but this time copy autocoder.mxo into ~/Documents/Max 8/Packages/autocoder/externals and libtensorflowlite_c.dylib into /usr/local/lib. If the directory doesn't exist then you can create it by opening the terminal and writing 
sudo mkdir /usr/local/lib.

# EXAMPLES

There are two example patches included, a [very basic one](https://github.com/franzson/autocoder_external/raw/main/examples/simple/autocoder-simple.zip) that works out of the box, and a [second more versatile one](https://github.com/franzson/autocoder_external/raw/main/examples/advanced/autocoder_advanced.zip) that requires Ircam's [spat](https://forum.ircam.fr/projects/detail/spat/) to be installed (spat is free to download if you set up a login on the Ircam forums).

Simply open either patch, hit load, select the soundfile in a downloaded model folder (it uses the name of the soundfile to keep track of files needed to be loaded), turn the audio processing on (the speaker button in the simpler patch, the 'audio on' button in the advanced example), and press the 'ON' button to start generating audio. 

You can alternatively generate latent vectors by adjusting the eight sliders in the patch.

Try playing with the brightness, smoothing, whitening, and auto_filter controls to get a sense of what they do. 

In the advanced patch, you can toggle the generator on/off by pressing 'GENERATE' and turn direct convolution or impulse reverb mode on by pressing 'DIR_CONVOLUTION' and 'REVERB' and hitting play on the audioplayer in the top right corner. You can also drag and drop your own soundfiles to use as input for the convolution process right there.

You can download example models [here](https://github.com/franzson/autocoder_models).

To train models for the autocoder, see [here](https://github.com/franzson/autocoder_training).

To train a model based on a corpus of sounds you will need to refer to the [python implementation of the autocoder](https://github.com/franzson/autocoder).

If the output of the models sounds sinusoidal or has a lot of artifacts, try lowering the whitening value. If they are boom-y or have resonant peaks, try increasing the brightness. If they sound noisy, try increasing the whitening value by a small amount.

Sinusoidal models are sometimes the result of overfitting during training so lowering the model quality and retraining may also solve the problem. Inversely, noisy models are sometimes the result of underfitting during training, so increasing the model quality and retraining may solve noise issues.

Different models sound better/worse at different generation intervals, you can adjust the range between latent space updates in the time section. A small scooping of the middle frequencies in the filter section is often helpful to produce a more natural sound.

If you want to compile the external by yourself, you need to first download and install the Max/MSP SDK and make sure it is all set up and then place the files in the /src/autocoder directory inside ~/Documents/Max 8/max-sdk-x.x.x/source/basics/autocoder, before building the examples with cmake. Rhen open autocoder.xcodeproj in XCode and add the libtensorflowlite_c.dylib to the linker.
