/*!
 *@file play.ino
 *@brief Music Playing Example Program
 *@details  Experimental phenomenon: control MP3 play music, obtain song information
 *@copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 *@license     The MIT license (MIT)
 *@author [fengli](li.feng@dfrobot.com)
 *@version  V1.1
 *@date  2021-10-15
 *@url https://github.com/DFRobot/DFRobot_DF1201S
*/


#include <DFRobot_DF1201S.h>

DFRobot_DF1201S DF1201S;

int potpin = A7;  // analog pin used to connect the potentiometer
int vol;    // variable to read the value from the analog pin


const char* file_paths[] = {
    "/meuh/Meuh1.mp3",
    "/meuh/Meuh2.mp3",
    "/meuh/Meuh3.mp3",
    // Add more paths as needed
};

const int num_files = sizeof(file_paths) / sizeof(file_paths[0]);

void setup(void){
  Serial.begin(9600);
  Serial.println("hello");

  Serial3.begin(115200);
  while(!DF1201S.begin(Serial3)){
    Serial.println("Init failed, please check the wire connection!");
    delay(1000);
  }
  //Set volume to 15
  DF1201S.setVol(/*VOL = */15);
  //deactivate prompts
  DF1201S.setPrompt(false);
  //Enter music mode
  DF1201S.switchFunction(DF1201S.MUSIC);
  //Set playback mode to "repeat all"
  DF1201S.setPlayMode(DF1201S.SINGLE);

}

void loop(){ 
  //vol = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023)
  //vol = map(vol, 0, 1023, 0, 30);     // scale it for use with the servo (value between 0 and 180)
  //DF1201S.setVol(/*VOL = */vol);

  //Play the file No.1, the numbers are arranged according to the sequence of the files copied into the U-disk 
  //DF1201S.playFileNum(/*File Number = */4);
  //Play the test.mp3 file in test folder 
  //DF1201S.playSpecFile("/meuh/Meuh7.mp3");


  // Play a random song from the list
  //playRandomSong();

  // Wait for some time before playing the next song
  delay(5000); // 10 seconds delay

}

void playRandomSong() {
  // Select a random file path from the list
  int randomIndex = random(num_files);
  const char* randomFile = file_paths[randomIndex];

  // Play the selected file
  DF1201S.playSpecFile(randomFile);
  Serial.print("Playing: ");
  Serial.println(randomFile);
}