#include <OnewireKeypad.h>

char KEYS[]= {
 '3','2','1',//'A',
 '6','5','4',//'B',
 '9','8','7',//'C',
 '#','0','*',//'D'
};

OnewireKeypad <Print, 12 > Keypad(Serial, KEYS, 4, 3, A3, 4700, 1000 );


void setup ()
{
 Serial.begin(115200);
 Keypad.SetHoldTime(100);  // Key held time in ms
 Keypad.SetDebounceTime(50); // Key Debounce time in ms
}


void loop() {

  char key = Keypad.Key_State();
  if(key==3){
    Serial.print("tecla: ");
    Serial.println((char)key);
    Serial.println(leitura());
    }

}

char leitura(){
  char keypress = Keypad.Getkey();  // put value of key pressed in variable 'keypress'
  while ((Keypad.Key_State())){}  // Stay here while Key is held down
  return keypress;
}

  
