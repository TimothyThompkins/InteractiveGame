
// set pin numbers:
const int right_tactile_pin = 2;     // the number of the pushtactile pin
const int left_tactile_pin = 3;
const int led_pin =  13;      // the number of the LED pin

// variables will change:
int left_tactile_state = 0;         // variable for reading the pushtactile status
int right_tactile_state = 0;
int incoming_byte = 0;   // for incoming serial data

void setup() {
  
  // initialize the LED pin as an output:
  pinMode(led_pin, OUTPUT);
  
  // initialize the pushtactile pin as an input:
  pinMode(left_tactile_pin, INPUT);
  pinMode(right_tactile_pin, INPUT);
  
  // opens serial port, sets data rate to 9600 bps
  Serial.begin(9600);     
}

void loop() {
  // read the state of the pushtactile value:
  left_tactile_state = digitalRead(left_tactile_pin);
  right_tactile_state = digitalRead(right_tactile_pin);
 
  // send data only when you receive data:
  if (Serial.available() > 0) {
          // read the incoming byte:
          incoming_byte = Serial.read();

          if (incoming_byte == 2){
              Serial.print("got_it ");
          }    
  }
 
  if (left_tactile_state == HIGH) {
  
    digitalWrite(led_pin, HIGH);
    Serial.write("left ");
  }
  else {
      digitalWrite(led_pin, LOW);
  }
  
  if (right_tactile_state == HIGH) {
    digitalWrite(led_pin, HIGH);
    Serial.write("right ");
    }
  else {
      digitalWrite(led_pin, LOW);
  }
}
