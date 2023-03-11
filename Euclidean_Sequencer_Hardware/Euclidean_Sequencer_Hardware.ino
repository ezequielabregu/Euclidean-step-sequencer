//magic formula 
// ((COUNT + ROTATION) * HITS) % STEPS
// < HITS 


#define ENCODER_DO_NOT_USE_INTERRUPTS
#include <Encoder.h>

#include <Bounce2.h>
Bounce debouncer1 = Bounce(); 
Bounce debouncer2 = Bounce();
Bounce debouncer3 = Bounce();
Bounce debouncer4 = Bounce();


#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE ();  //creo la instancia

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//LiquidCrystal_I2C lcd(0x3F,20,4);  //New liquidCrystal library
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

//variables
//int steps= 0;
//int hits= 0;
//int rotation= 0;
int euclStep= 0;
int euklid= 0;
//int accent= 0;
//int rotationAcc=0;
//int note=0;
int keyVel=0;
int euclAcc=0;
int accentState=0;
int indexAcc=0;
int noteAcc=0;
int lastNoteState=0;
int noteState=0;
int lastIndexAcc=0; 
int euclidFull = 0;
int countCompare=0;
int euklidState=0;
int euclidAccent=0;
int count=0;

//Arrays 
int euclidSerie [56];
int euclidAcc [56];
int euclidArray [56]; 
int potArray[56];
int potVal[16];
int lastPot[16];
int lastPotVal[16];
int divBPM[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 16, 24, 32, 64}; 

int encArray[16];
int lastEncVal [56];

//Sequencer Timing
unsigned long previousMicros1 = 0;
unsigned long previousMicros2 = 0;
unsigned long previousMicros3 = 0;
unsigned long previousMicrosMaster = 0;
unsigned long previousMicrosLED = 0;
unsigned long previousMicrosCLOCK = 0;

long intervalMs = 150000;

int noteNum1= 0;
int noteNum2= 0;
int noteNum3= 0;
//int index=0;

int lastPot1 = 0;
int lastPot1Val=0;
int pot1Val=0;
int lastPot2 = 0;
int lastPot2Val=0;
int pot2Val=0;
int lastPot3 = 0;
int lastPot3Val=0;
int pot3Val=0;


int Threshlod = 7;

int pot1=0;
int pot2=0;
int pot3=0;
int pot4=0;
int pot5=0;
int pot6=0;
int pot7=0;
int pot8=0;

int countPlot=0;
int countPlotClear=0;
int countAcc=0;

int euclidVal= 0;

//int pot=0;
//int lastPot=0;
//int potVal=0;
//int lastPotVal=0;

///BUTTONS
int lastButtonVal1=0;
int lastButtonVal2=0;
int lastButtonVal3=0;
int lastButtonVal4=0;
int buttonCount=0;

int offset = 0;
int displayCursor = 1;
int button4State = 0;
//ENCODERS
//Encoder enc1 (38, 40); // STEPS
//Encoder enc2 (42, 44); // HITS
//Encoder enc3 (46, 48); // ACCENT
//Encoder enc4 (50, 52); // ROTATION
//Encoder enc5 (25, 23); // ROT ACCENT
//Encoder enc6 (29, 27); // Note Number
//Encoder enc7 (26, 24); // BPM
//Encoder enc8 (30, 28); // DIV

Encoder enc6 (38, 40); // NOTE NUMBER
Encoder enc1 (42, 44); // STEPS
Encoder enc2 (46, 48); // HITS
Encoder enc3 (50, 52); // ACCENTS
Encoder enc7 (25, 23); // BPM
Encoder enc8 (29, 27); // DIV
Encoder enc4 (26, 24); // ROTATION
Encoder enc5 (30, 28); // ROTATION ACCENT


long encPos[]  = {0,-999,-999,-999,-999,-999,-999,-999, -999};

int potDivBPM;

//CUSTOM CHARACTER
byte customChar0[] = { //death notes
  B00000,
  B00000,
  B00000,
  B00100,
  B00000,
  B00000,
  B00000,
  B00000
};

byte customChar1[] = { //HITS
  B00000,
  B01110,
  B10001,
  B10001,
  B10001,
  B10001,
  B01110,
  B00000
};

byte customChar2[] = { //ACCENTS
B00000,
  B01110,
  B11111,
  B11011,
  B11011,
  B11111,
  B01110,
  B00000
};

byte customChar3[8] = { //BPM
  B01000,
  B11000,
  B01001,
  B01010,
  B00100,
  B01000,
  B10000,
  B00000
};

byte customChar4[8] = { // channel 1
  B11011,
  B10011,
  B11011,
  B11011,
  B11011,
  B11111,
  B00000,
  B00000
};

byte customChar5[8] = { //Channel 2
  B10001,
  B11101,
  B10001,
  B10111,
  B10001,
  B11111,
  B00000,
  B00000
};

byte customChar6[8] = { //Channel 3
  B10001,
  B11101,
  B11001,
  B11101,
  B10001,
  B11111,
  B00000,
  B00000
};

byte customChar7[8] = { // Flecha
 B00000,
  B00100,
  B01000,
  B11111,
  B01000,
  B00100,
  B00000,
  B00000
};

///INFO DISPLAY
int bpmDisplay=1;

int noteDisplay = 0;

int index1=0;
int index2=0;
int index3=0;
int note1=0;
int note2=0;
int note3=0;

int div1 = 1;
int div2 = 1;
int div3 = 1;
int lastDiv1=1;

int selectChannel = 0;

int ledState = LOW; 
int cursorState= 0;
int countSync = 0;



///////////////////////////////////////////////////////
void setup() {
  Serial.begin (31250);
  lcd.begin(20,4); 
//  lcd.init(); // Init LCD
//  lcd.backlight(); 
MIDI.begin(MIDI_CHANNEL_OMNI); // MIDI Library (old) init 


//CUSTOM CHARACTERS SETUP
lcd.createChar(0, customChar0);
lcd.createChar(1, customChar1);
lcd.createChar(2, customChar2);
lcd.createChar(3, customChar3);
lcd.createChar(4, customChar4);
lcd.createChar(5, customChar5);
lcd.createChar(6, customChar6);
lcd.createChar(7, customChar7);

Wire.setClock(400000); // increase I2C clock for more stable MIDI clock  

 // Setup the button with an internal pull-up :
pinMode(2,INPUT_PULLUP);
pinMode(3,INPUT_PULLUP);
pinMode(4,INPUT_PULLUP);
pinMode(5,INPUT_PULLUP);

debouncer1.attach(2); //debounce Digital Input
debouncer1.interval(5); // interval in ms 
debouncer2.attach(3); 
debouncer2.interval(5);
debouncer3.attach(4); 
debouncer3.interval(5);
debouncer4.attach(5); 
debouncer4.interval(5);

pinMode (13, OUTPUT);


//////////// BOOT ANIMATION /////////////////////////////////
lcd.setCursor(0, 0);
lcd.print("**** LUCLYDIAN ****");
lcd.setCursor(0, 1);
delay(500);
lcd.print("Step Sequencer &");
lcd.setCursor(0, 2);
delay(500);
lcd.print("MIDI controller");
lcd.setCursor(0, 3);
delay(500);
lcd.print("FMW V250119 E ABREGU");
delay (1000);

for (int countScroll = 0; countScroll < 19; countScroll++) {
      lcd.scrollDisplayRight();
      delay(10);
                 }
                 
lcd.clear();

lcd.setCursor(3, 2);
lcd.print("Let's Step!");

///Force display initial value for channel 2 and 3
display_note( 36, 2);
display_note( 36, 3);
displayOctave (36,2); 
displayOctave (36,3);

/////ENCODER 6 INIT VALUE (MIDI NOTE) ////
int enc6Init = 36;  ///> 36 as Init Value for MIDI N0te
enc6.write (enc6Init * 4);
lastEncVal[6]= 36;
lastEncVal[6+20]= 36;
lastEncVal[6+40]= 36;

}

///////////////LOOP///////////////////////////////////////
void loop() {

///Debounce & Control Buttons/////////////////////////////////
debouncer1.update();
debouncer2.update();
debouncer3.update();
debouncer4.update();

int buttonVal1 = debouncer1.read(); //Button Debounced
int buttonVal2 = debouncer2.read();
int buttonVal3 = debouncer3.read();
int buttonVal4 = debouncer4.read();

if (buttonVal1 != lastButtonVal1){
  if (buttonVal1 == 1){
    offset = 0;
    displayCursor = 1;
    }
  }
if (buttonVal2 != lastButtonVal2){
  if (buttonVal2 == 1){
          offset = 20; //20 - 1 element
          displayCursor = 2;          
      }
  }
if (buttonVal3 != lastButtonVal3){
  if (buttonVal3 == 1){
          offset = 40; //40 - 1 element
          displayCursor = 3;          
      }
  }
if (buttonVal1 != lastButtonVal1 || buttonVal2 != lastButtonVal2 || buttonVal3 != lastButtonVal3){
    if (buttonVal1 == 1 || buttonVal2 == 1 || buttonVal3 == 1){

         //displaySelect (displayCursor);
         
          enc1.write (lastEncVal [1+offset] * 4);
          enc2.write (lastEncVal [2+offset] * 4); 
          enc3.write (lastEncVal [3+offset] * 4); 
          enc4.write (lastEncVal [4+offset] * 4); 
          enc5.write (lastEncVal [5+offset] * 4);
          enc6.write (lastEncVal [6+offset] * 4);
          enc8.write (lastEncVal [8+offset] * 4);   
     }       
   }

if (buttonVal4 != lastButtonVal4){
  if (buttonVal4 == 1){
  button4State++; 
    if (button4State % 2 == 1){
        
       MIDI.sendRealTime(midi::Start);       
      }
    else {
        
        MIDI.sendRealTime(midi::Stop);
        }
      }
  }
  
lastButtonVal1 = buttonVal1;
lastButtonVal2 = buttonVal2;
lastButtonVal3 = buttonVal3;
lastButtonVal4 = buttonVal4;         

 
//Serial.println(buttonCount);
//Serial.print(" --- ");
//Serial.println(offset);
//Serial.print(" --- ");
//Serial.println(displayCursor);      


///ENCODERS/////////////////////////////////////////////
long newEnc1, newEnc2, newEnc3, newEnc4, newEnc5, newEnc6, newEnc7, newEnc8;  

newEnc1= enc1.read()/4;
newEnc2= enc2.read()/4;
newEnc3= enc3.read()/4;
newEnc4= enc4.read()/4;
newEnc5= enc5.read()/4;
newEnc6= enc6.read()/4;
newEnc7= enc7.read()/4;
newEnc8= enc8.read()/4;

byte maxRange = 16;
byte minRange = 0;
byte maxNoteRange = 127;
byte maxBPM = 500;


if (newEnc1 > maxRange){ newEnc1=maxRange; enc1.write(maxRange*4); } // Limit range mutiplied x 4 position (oposite to enc1.read()/4)
if (newEnc1 < minRange){ newEnc1=0; enc1.write(0); }
if (newEnc2 > newEnc1){ newEnc2=newEnc1; enc2.write(newEnc1 * 4); } // HITS 
if (newEnc2 < minRange){ newEnc2=0; enc2.write(0); }
if (newEnc3 > newEnc2){ newEnc3=newEnc2; enc3.write(newEnc2 * 4); } //ACCENTS
if (newEnc3 < minRange){ newEnc3=0; enc3.write(0); }
if (newEnc4 > newEnc1){ newEnc3=newEnc1; enc4.write(newEnc1 * 4); } //ROTATION
if (newEnc4 < minRange){ newEnc4=0; enc4.write(0); }
if (newEnc5 > newEnc3){ newEnc5=newEnc3; enc5.write(newEnc3*4); } //ACCENTS ROTATION
if (newEnc5 < minRange){ newEnc5=0; enc5.write(0); }

if (newEnc6 > maxNoteRange){ newEnc6=maxNoteRange; enc6.write(maxNoteRange * 4); } //MIDI NOTES
if (newEnc6 < minRange){ newEnc6=0; enc6.write(0); }
if (newEnc7 > maxBPM){ newEnc7 = maxBPM; enc7.write(maxBPM * 4); } //BPM
//if (newEnc7 < minRange){ newEnc7=0; enc7.write(0); }
if (newEnc8 > maxRange){ newEnc8=maxRange; enc8.write(maxRange * 4); } //DIVISION
if (newEnc8 < minRange ){ newEnc8= minRange; enc8.write(0); }

encArray[1]= newEnc1;
if (encArray[1] != encPos[1]) {
      lastEncVal [1+offset]= encArray[1]; //--> save last Value into array
      count=0;
      countPlotClear=encArray[1];       
      encPos[1] = encArray[1];
        }

encArray[2]= newEnc2;
if (encArray[2] != encPos[2]) {
      lastEncVal [2+offset]= encArray[2]; //--> save last Value into array
//      Serial.print(encArray[2]);        
//      Serial.print(" --- ");
//      Serial.println(lastEncVal [2+offset]);
      count=0;        
      countPlotClear=encArray[1];            
      encPos[2] = encArray[2];
        }    
    
encArray[3]= newEnc3;
if (encArray[3] != encPos[3]) {
      lastEncVal [3+offset]= encArray[3]; //--> save last Value into array          
      count=0;        
      countPlotClear=encArray[1];        
      encPos[3] = encArray[3];
        }

encArray[4]= newEnc4;
if (encArray[4] != encPos[4]) {
      lastEncVal [4+offset]= encArray[4]; //--> save last Value into array        
      count=0;        
      countPlotClear=encArray[1];
      encPos[4] = encArray[4];
        }

encArray[5]= newEnc5;
if (encArray[5] != encPos[5]) {
      lastEncVal [5+offset]= encArray[5]; //--> save last Value into array    
      count=0;        
      countPlotClear=encArray[1]; 
      encPos[5] = encArray[5];
        }
        
encArray[6]= newEnc6; //MIDI NOTE
if (encArray[6] != encPos[6]) { 
      
      lastEncVal [6+offset]= encArray[6];  
      display_note(lastEncVal [6+offset], displayCursor);
      displayOctave (lastEncVal [6+offset], displayCursor);
      encPos[6] = encArray[6];
        }
        
encArray[7]= newEnc7; //BPM
if (encArray[7] != encPos[7]) {
       bpmDisplay=1;                  
      encPos[7] = encArray[7];
        }

encArray[8]= newEnc8; //DIV
if (encArray[8] != encPos[8]) {
      lastEncVal [8+offset]= encArray[8]; 
      //divBPM[8+offset]= lastEncVal [8+offset];
      //Serial.print(encArray[8]); 
      displayDiv (divBPM [lastEncVal[8]], divBPM [lastEncVal[8+20]],divBPM [lastEncVal[8+40]] );                 
      encPos[8] = encArray[8];

      } 

intervalMs = ((60000/(encArray[7] + 120))) * 1000; 
 

div1 = divBPM [lastEncVal [8] ]; 
div2 = divBPM [lastEncVal [8+20] ];
div3 = divBPM [lastEncVal [8+40] ];

//Serial.print (div1);
//Serial.print (" --- ");
//Serial.print (div2);
//Serial.print (" --- ");
//Serial.println (div3);

///FUNCTIONS CALLS////////////////////////
   
EuclidEngine(encArray[1], encArray[2], encArray[3], encArray[4], encArray[5]); //Euclidean Seq Function  

displaySeq(encArray[1] , displayCursor);

displayInfo (encArray[7], displayCursor);


noteNum1=lastEncVal[6];
noteNum2=lastEncVal[6+20];
noteNum3=lastEncVal[6+40];
 
unsigned long currentMicros1 = micros(); ///necesita un currentMicro para cada instancia
unsigned long currentMicros2 = currentMicros1;//micros();
unsigned long currentMicros3 = currentMicros1; //micros();
unsigned long currentMicrosMaster = currentMicros1;//micros();
unsigned long currentMicrosLED = currentMicros1;// micros();
unsigned long currentMicrosCLOCK = micros();

  
 //**//**//**CHANNEL 1//**//**//**

if (currentMicros1 - previousMicros1 >= intervalMs / div1) {
    previousMicros1 = currentMicros1;
    
  if (index1 >= lastEncVal[1]){
         index1=0;   
          }          
    if (button4State % 2 == 1){ //if Button 4 is 1, play it!
    index1++;    
    note1= euclidArray[(index1-1)]; 
    if (note1 == 1) {
     MIDI.sendNoteOff (noteNum1,100,1);
     MIDI.sendNoteOn (noteNum1,100,1);
       }
    if (note1 == 2) {
     MIDI.sendNoteOff (noteNum1,127,1);
     MIDI.sendNoteOn (noteNum1,127,1);
       }   
    }
}
 //**//**//**CHANNEL 2//**//**//**
if (currentMicros2 - previousMicros2 >= intervalMs / div2) {
    previousMicros2 = currentMicros2;
  if (index2 >= lastEncVal[1+20]){
         index2=0;   
          }          
 if (button4State % 2 == 1){
    index2++;    
    note2= euclidArray[(index2-1)+20]; 
    if (note2 == 1) {
     MIDI.sendNoteOff (noteNum2,100,1);
     MIDI.sendNoteOn (noteNum2,100,1);
       }
    if (note2 == 2) {
     MIDI.sendNoteOff (noteNum2,127,1);
     MIDI.sendNoteOn (noteNum2,127,1);
       }
    }
}
 //**//**//**CHANNEL 3//**//**//**
if (currentMicros3 - previousMicros3 >= intervalMs / div3) {
    previousMicros3 = currentMicros3;
  if (index3 >= lastEncVal[1+40]){
         index3=0;   
          }          
 if (button4State % 2 == 1){
    index3++;    
    note3= euclidArray[(index3-1)+40]; 
    if (note3 == 1) {
     MIDI.sendNoteOff (noteNum3,100,1);
     MIDI.sendNoteOn (noteNum3,100,1);
       }
    if (note3 == 2) {
     MIDI.sendNoteOff (noteNum3,127,1);
     MIDI.sendNoteOn (noteNum3,127,1);
       } 
    }
}
 //**//**//**MASTER CLOCK//**//**//**
if (currentMicrosMaster - previousMicrosMaster >= intervalMs) {
    previousMicrosMaster = currentMicrosMaster;
  
  previousMicros1 = 0;      
  previousMicros2 = 0;
  previousMicros3 = 0;
  previousMicrosCLOCK = 0;

  if (cursorState == LOW) {
      cursorState = HIGH;
      displaySelect (displayCursor);
    } 
    else {
      cursorState = LOW;
      lcd.setCursor(19, displayCursor);
      lcd.print(" ");
   
    }
  }    
//////////LED BPM FEEDBACK ///////////////////
if (currentMicrosLED - previousMicrosLED >= intervalMs / 2) {
   previousMicrosLED = currentMicrosLED;
   if (ledState == LOW) {
      ledState = HIGH;     
    } 
    else {
      ledState = LOW;
    }
  if (button4State % 2 == 1){ //If play button 4 is ON, blink it!
  digitalWrite (13, ledState);  
  }
  else {digitalWrite (13, LOW);}
}
///////////CLOCK MIDI/////////////////////////////
if (currentMicrosCLOCK - previousMicrosCLOCK >= intervalMs / 24){
   previousMicrosCLOCK = currentMicrosCLOCK;
//   countSync++;
//   if (countSync > 23) {
//     previousMicros1 = 0;      
//     previousMicros2 = 0;
//     previousMicros3 = 0;
//     countSync = 0;     
//   }
   MIDI.sendRealTime(midi::Clock);   
    } 
  
}
//////////////////////FUNCTIONS////////////////////////////

/////////MIDI GENERATOR///////////////////////////

      
           
  


////////////EUCLID ENGINE //////////////////////////////////////////////////////////
void EuclidEngine (int stepsNum, int hits, int accent, int rotation, int rotationAcc) {
 

////////////////////////////////////////////////

while (count < stepsNum)
    {      
      count++;  
      // le resto -1  a count para que comience en la ecuación en 0, sino lo rota
      euclStep = (((count-1) + rotation) * hits) % stepsNum; 
      if (euclStep < hits) {
        euklid = 1;         
        }
      else {
        euklid = 0;
      }
      euclidSerie[(count-1) + offset] = euklid;
     
      if (count == stepsNum){ countAcc= 0; countCompare= 0;} //cuando termina este while, pasa a los siguiente

//     Serial.print(euclidSerie[(count-1) + offset]);
//      Serial.print(" ");
//      Serial.print(stepsNum);     
//      Serial.print("----");       
    }                       
//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--

while (countAcc < hits)
     { 
      countAcc++;
       
      euclAcc = (((countAcc-1) + rotationAcc) * accent) % hits;
      if (euclAcc < accent) {
        accentState = 1;
        }
      else {
        accentState = 0;
      }
      euclidAcc[(countAcc-1) + offset] = accentState; 
      euclidAccent = euclidAcc[countAcc-1 ];  //----> REVISAR porque no lo estoy usando

//     if (countAcc == hits){countCompare= 0; } //-->countCompare empieza en -1 para evitar artefactos (retraso un índex la comparación para que quede sincronizado)

//       Serial.println(countAcc);
//      Serial.print("_");
       }        
//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--      
  
  while (countCompare < stepsNum)
{
countCompare++;
//Serial.print(countCompare);
  
euclidFull = euclidSerie[(countCompare-1)+offset]; 

   if (euclidFull == 1) {        
        indexAcc++;                        
        noteAcc= euclidAcc[(indexAcc-1)+offset];
               
        if (noteAcc == 1){
         euclidFull = 2;       
                 }                                                               
               }             
   // Serial.println(indexAcc);                    
      if (indexAcc >= hits){  
         indexAcc=0; } 
                                                                                   
 euclidArray[(countCompare-1)+offset]= euclidFull; 

if (countCompare == stepsNum) {
  countPlot= 0; 
  displaySeq(encArray[1], displayCursor); //run display function when euclidArray is done
  } 

//Serial.print(euclidArray[(countCompare-1)+offset]);  
//Serial.print(stepsNum);
//Serial.print(" --- ");
//Serial.println(countCompare-1);
//Serial.print(" --- ");
//Serial.print(euclidFull);
//Serial.print("");          
  
  }                                        
}       
//////////////DISPLAY FUNCTION////////////////////////////////

void displaySeq(int steps, int plotPos) {
   
while (countPlot  < steps)
{
 countPlot++;  
 
  int indexPlot=euclidArray[(countPlot-1)+offset];
  
 if (indexPlot ==0){
 lcd.setCursor(countPlot-1, plotPos);
 lcd.print(char(0));
      }
 if (indexPlot ==1){
 lcd.setCursor(countPlot-1, plotPos);
 lcd.print(char(1));
      }
 if (indexPlot ==2){
 lcd.setCursor(countPlot-1, plotPos);
 lcd.print(char(2));
      }
//Serial.print(steps);
//Serial.print(" --- ");
//Serial.print(countPlot-1);
//Serial.println("");        
}


while (countPlotClear < 16)
{
countPlotClear++;

 lcd.setCursor(countPlotClear-1, plotPos);
 lcd.print(" ");

//Serial.println(countPlotClear-1); 

      }       

if (steps==0){  //clear LCD if steps = 0 (sino queda un punto)
lcd.setCursor(0, plotPos);
lcd.print(" ");     
  }
}

/////////////DISPLAY INFORMATION/////////////////////////
void displayInfo (int tempo,int plotPos){

while (bpmDisplay == 1){ //BPM
lcd.setCursor(0, 0);
lcd.print ("BPM>"); //(char(3));
lcd.setCursor(4, 0);
lcd.print(tempo + 120);
if ((tempo) < 100) lcd.print (" "); 
 bpmDisplay = 0;
  } 
}

///////////////////////////////////////////////

void displaySelect (int plotPos){
if (plotPos == 1){
  lcd.setCursor(19, 1);
  lcd.print(char(7));}
else {lcd.setCursor(19, 1);
  lcd.print(" ");}

if (plotPos == 2){
  lcd.setCursor( 19, 2);
  lcd.print(char(7));}
else {lcd.setCursor(19, 2);
  lcd.print(" ");}

if (plotPos == 3){
  lcd.setCursor(19, 3);
  lcd.print(char(7));}
else {lcd.setCursor(19, 3);
  lcd.print(" ");}  
}

////////////////////////////////////////////////////////////////////////////////

void displayDiv (int divPlot1, int divPlot2, int divPlot3){

lcd.setCursor(10, 0);
lcd.print(char(3));
lcd.setCursor(11, 0);
lcd.print(char(4));
lcd.setCursor(12, 0);
lcd.print(divPlot1);
if ((divPlot1) < 100) lcd.print (" ");

lcd.setCursor(14, 0);
lcd.print(char(5));
lcd.setCursor(15, 0);
lcd.print(divPlot2);
if ((divPlot2) < 100) lcd.print (" ");

lcd.setCursor(17, 0);
lcd.print(char(6));
lcd.setCursor(18, 0);
lcd.print(divPlot3);
if ((divPlot3) < 100) lcd.print (" ");
}
//////////////////////////////////////////////////////////////////////////////
void display_note(int value, int notePos) {
  int notes_num = 12;
  String notes_name[notes_num] = {"C ","C#","D ","D#","E ","F ","F#","G ","G#","A ","A#","B "};
  String note;
  
  for (int i = 0; i < notes_num; i++)  
    if(value % notes_num == i) {
      note = notes_name[i];
    lcd.setCursor(16, notePos);
    lcd.print(note);  
    }
}

///////////////////////

void displayOctave (int value, int notePos){
//int octaveNum = 10;
int octaveArray [] = {0,1,2,3,4,5,6,7,8,9,10};
int octave;
int indexOctave;
indexOctave=0;
for (int ioctave = 0; ioctave < value; ioctave++)  
    if (ioctave % 12 == 11) {
      indexOctave++;
    octave = octaveArray[indexOctave-1];    
   
    lcd.setCursor(18, notePos);
    lcd.print(octave);  
    }

}    
