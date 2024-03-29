# KEYVILBOARD
All KEYVILBOARD modules are fully opensource and are hand assambled in the Netherlands. Thank you for buying one of our modules if have not yet bought one and would like one go to https://keyvilboard.nl. 

**If you have any improvements to our code or found a bug please create an issue or pull request!**

If you want to contact us, please email us via [info@keyvilboard.nl](mailto:info@keyvilboard.nl).

## Why would you buy a KEYVILBOARD and not just some other keylogger?

- It's open source

Most people don't realise this but all the commercial keyloggers that we found online are closed source. You have no clue what the software is actually doing with the keystrokes that are logged. It could just as well be sending it to a remote server, you just don't know for sure. We provide you with the ability to review the code for yourself, but also to burn it yourself onto the chips. **With the KEYVILBOARD you have full control of what software is running on it.**

- It's cheaper than most alternative options

This project initialy started due too the high prices online for keyloggers that were from shady companies and were limited in their features. Now, after months of development and (fun!) frustration, we can provide you with a **cheaper, more transparent and fundamentally the ideal keylogger**.  

- You can remotely access the KEYVILBOARD via SMS or Wi-Fi (depending on the module you bought)

Most keyloggers only store the keystrokes locally, which requires you to have physical access at least two times instead of just one time. There are some keyloggers that support remote Wi-Fi access, but the software is closed source and the antenna too small causing short range. With the KEYVILBOARD you can access the device from **practically anywhere in the world** using SMS for long term operations and from an actual safe distance using Wi-Fi for ad-hoc operations. Besides that we also have other cool features like **keystroke injection that no other keylogger on the market currently has**.

## How to change the name of the keyvilboard so it is not Arduino Leonardo?
1. Make sure all Arduino instances are closed.
2. Navigate to Program Files (x86)\Arduino\hardware\arduino\avr.
3. Open "boards.txt" with a text editor. 
4. Search for ATMEGA32u4
5. Change VID/PID and name Accordingly to what you want to be.

## KEYVILBOARD.STL
This file contains the 3d printed case that is also delivered with your KEYVILBOARD, it fits for both SMS & Wi-Fi editions. 

## Hardware designs
As we don't produce any more KEYVILBOARD's, we thought it would be appropriate to share the hardware designs - so others may build further upon it. For those interested, you may find the required information here; https://oshwlab.com/helmmen/usbhost

## SMS Version
This folder contains the instructions, code + libary used within the SMS version.

## Wi-Fi Version
This folder contains the instructions, code + libary used within the Wi-Fi version.

## Credits (no specific order)
Special thanks to [Yago Hansen](https://github.com/yadox666), [Luca Bongiorni](https://github.com/whid-injector), [Jesse Vincent](https://github.com/obra), [michalmonday](https://github.com/michalmonday) and [spacehuhn](https://github.com/spacehuhn) for the code we were able to implement in our code. 
