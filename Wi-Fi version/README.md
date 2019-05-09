# KEYVILBOARD Wi-Fi Setup

Out of the box the KEYVILBOARD Wi-Fi has the following settings:

- Visible network
- SSID: KEYVILBOARD
- Password: keylogger

After inserting the KEYVILBOARD Wi-Fi you can connect to the created hotspot (192.168.4.1) and configure these settings in the settings menu. It is advised to change the network to hidden, to prevent other user from getting alarmed, and obvisously to change the password.

**Only connect the keyboard after the KEYVILBOARD has been inserted into the USB port for a few seconds. Otherwise the keyboard and KEYVILBOARD WILL NOT work. This is due to a capacitor on the board that was added for stability.**

## Updating the KEYVILBOARD Wi-Fi

Updating the KEYVILBOARD Wi-Fi is actually quite easy. Just go to the webinterface, click on info in the menu and click on the file button to select a new bin file which you then need to upload. If everything was uploaded successfully then you should a blank page with OK.

You can find the most recent .bin file for your ESP8266 chip in the esp8266_chip/keyvilboard_wifi folder.

## Features

Just like the KEYVILBOARD SMS you can log keystrokes and inject them. Provided in the webinterface of the KEYVILBOARD Wi-Fi you can easily see, download or erase the keylogging file but you can also execute Rubber Ducky code and save scripts.

Why is key stroke injection useful for a keylogger? Well for one you can manipulate the user easier to provide you with sensitive information. You could for example lock the PC to force the user to immediatly provide you with user credentials, or create a fake login pop-up where the user needs to provide Administrator credentials. Besides that you can also use all the other available scripts, the sky is the limit.

Here are some great sources with Rubber Ducky scripts:

- https://ducktoolkit.com
- https://github.com/hak5darren/USB-Rubber-Ducky/wiki/Payloads
