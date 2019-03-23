# KEYVILBOARD SMS SETUP/Functions

**Only connect the keyboard after the KEYVILBOARD has been inserted into the USB port for a few seconds. Otherwise the keyboard and KEYVILBOARD WILL NOT work. This is due to a capacitor on the board that was added for stability.**

1. SIM Setup

* SIM Lock remove

     Make sure your remove the simlock from the simcard you intent to use in Module easy way to do that is by putting it in an older           phone     and removing it their not all new phones have the option.
  
* SIM Advice
  
     I reccomend you to take a unlimited texting subscription these will cost about 7-8 euro's a month monthly cancelable keep in mind         these are trackable so dont be doing anything illigal with our modules.
  
* Conclusion
  
    If you have completed all these steps and you put the sim card inside of the simcard holder with the metal pads facing towards the pcb     and the cut corner aiming towards the male USB(You can seen that in our product pictures if you don't understand)the GSM module           should start blinking once every 3 seconds if it blinks every second make sure your simcard works and has mobile connection.

2. KEYVILBOARD Setup

* Arduino
     
     Our code works with the arduino leonardo bootloader make sure you download the arduino IDE and you have a simple understand of how it works.
      
     The code is set and done Furthermore we have allready prepared and tested all modules that go out the door all you have to do is add your phone number in the 2 text send methods and upload it with the arduino leonardo board and com-port selected.

3. Explination of Functions

* Code
     
     * The keylogging will be at all times persistence the subject will never notice anything different since that the modules are using a raw HID constant throughput no delay no change. We have even tested them while gaming and noticed no difference. Thanks to michalmonday for making this possible with his library.
     
     * The code contains some payload methods if you do not own the right to use those, don’t! These payload methods contain the possibility to execute any kind of evil keyboard style attack while also being smart. Since it captures the windows password if that password is enterd the wrong the first time your able to manually change it with the ManualPass: function. Everything after the ":" will be the password you can also change and implement these methods in diffrent ways and apply them to other login forms. With these modules you have a fully airgaped way of listening and an unlimited time vector for when to attack. 
     
     
     * The payload methods work by listing to text messages being send to the gsm module and containing certaint keywords. So for starting the example payload that is provided in the code you will send a text message to the module that says "Execute:Payload" once this string is received by the module and the subject hasn't typed for 15 seconds it will execute the payload. 
     
     * We are using 32kb atmega32u4 chips with the current code their is about 15kb left for additional payloads.
     
     * What makes this module extra cool is that is pretty much is smart keylogging although some passwords and usernames must be manually filled in with ManualPassword functions it makes it possible to exfill data threw a user’s own input. A cool proof on concept would be to send certaint files that are for instance classified threw the email of the subject itself. It's possible to all this without ever running any code on the system of the subject.
 
 
     
     
     
     
