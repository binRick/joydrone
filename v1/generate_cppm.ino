#include <serial-readline.h>
#include <Arduino_JSON.h>




void received(char*);
SerialLineReader reader(Serial, received);





//////////////////////CONFIGURATION///////////////////////////////
#define CHANNEL_NUMBER 6  //set the number of chanels
#define CHANNEL_DEFAULT_VALUE 1500  //set the default servo value
#define FRAME_LENGTH 22500  //set the PPM frame length in microseconds (1ms = 1000µs)
#define PULSE_LENGTH 300  //set the pulse length
#define onState 1  //set polarity of the pulses: 1 is positive, 0 is negative
#define sigPin 3  //set PPM signal output pin on the arduino
int ppm[CHANNEL_NUMBER];

/*this array holds the servo values for the ppm signal
 change theese values in your code (usually servo values move between 1000 and 2000)*/


void received(char *line) {
    JSONVar myObject = JSON.parse(line);
    int roll = (int)myObject["roll"];
    int pitch = (int)myObject["pitch"];
    int yaw = (int)myObject["yaw"];
    int throttle = (int)myObject["throttle"];
    int button0 = (int)myObject["button0"];
    ppm[3]=map(roll,0,2000,1000,2000);
    ppm[1]=map(pitch,0,2000,1000,2000);
    ppm[0]=map(yaw,0,2000,1000,2000);
    ppm[2]=map(throttle,0,2000,1000,2000);
    ppm[5]=2000;
    //ppm[4]=map(roll,0,2000,1000,2000);

    //ppm[4]=button0;


    //Serial.println((int) myObject["roll"]);

}


void setup(){  
	Serial.begin(115200);

  //initiallize default ppm values
  for(int i=0; i<CHANNEL_NUMBER; i++){
      ppm[i]= CHANNEL_DEFAULT_VALUE;
  }

  pinMode(sigPin, OUTPUT);
  digitalWrite(sigPin, !onState);  //set the PPM signal pin to the default state (off)
  
  cli();
  TCCR1A = 0; // set entire TCCR1 register to 0
  TCCR1B = 0;
  
  OCR1A = 100;  // compare match register, change this
  TCCR1B |= (1 << WGM12);  // turn on CTC mode
  TCCR1B |= (1 << CS11);  // 8 prescaler: 0,5 microseconds at 16mhz
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  sei();

}

void loop(){
  	reader.poll();


  /*
    Here modify ppm array and set any channel to value between 1000 and 2000. 
    Timer running in the background will take care of the rest and automatically 
    generate PPM signal on output pin using values in ppm array
  */
  
}

ISR(TIMER1_COMPA_vect){  //leave this alone
  static boolean state = true;
  
  TCNT1 = 0;
  
  if (state) {  //start pulse
    digitalWrite(sigPin, onState);
    OCR1A = PULSE_LENGTH * 2;
    state = false;
  } else{  //end pulse and calculate when to start the next pulse
    static byte cur_chan_numb;
    static unsigned int calc_rest;
  
    digitalWrite(sigPin, !onState);
    state = true;

    if(cur_chan_numb >= CHANNEL_NUMBER){
      cur_chan_numb = 0;
      calc_rest = calc_rest + PULSE_LENGTH;// 
      OCR1A = (FRAME_LENGTH - calc_rest) * 2;
      calc_rest = 0;
    }
    else{
      OCR1A = (ppm[cur_chan_numb] - PULSE_LENGTH) * 2;
      calc_rest = calc_rest + ppm[cur_chan_numb];
      cur_chan_numb++;
    }     
  }
}