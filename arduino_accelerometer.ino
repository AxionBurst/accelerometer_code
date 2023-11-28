
const int xpin = A0;                  // x-axis
const int ypin = A1;                  // y-axis
const int zpin = A2;                  // z-axis
const int ledPin = 9;

const int buttonPin = 10;
int buttonState = 0;

const int num_cal_readings = 35;

float x_cal;
float x_res;
float x_0;

int x_cal_baseline_array[num_cal_readings];
int y_cal_baseline_array[num_cal_readings];
int z_cal_baseline_array[num_cal_readings];

int x_cal_1g_array[num_cal_readings];
int y_cal_1g_array[num_cal_readings];
int z_cal_1g_array[num_cal_readings];

void setup() {

  // initialize the serial communications:

  Serial.begin(9600);

  delay(2000);

  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  calibrate_axis_x();

}

void loop() {

  // print the sensor values:

  char buffer[40];
  sprintf(buffer, "This is the calibration value: %d", x_cal);
  Serial.println(buffer);

  sprintf(buffer, "This is the resolution average: %d", x_res);
  Serial.println(buffer);

  sprintf(buffer, "This is the baseline average: %d", x_0);
  Serial.println(buffer);

  float x_val;
  x_val = analogRead(xpin - x_0);

  Serial.print(x_val * x_res);

  // print a tab between values:a

  Serial.print("\t");

  Serial.print(analogRead(ypin));

  // print a tab between values:

  Serial.print("\t");

  Serial.print(analogRead(zpin));
 
  Serial.print("\t");

  Serial.print("\n");

  // delay before next reading:

  delay(1000);
}

void calibrate_axis_x() {

  Serial.println("Starting calibration, collecting baseline data, hold x-axis flat");

  //delay(3000);
  while (buttonState != HIGH){
    buttonState = digitalRead(buttonPin);
    delay(50);
  }

  digitalWrite(ledPin, HIGH);
  buttonState = LOW;

  for (byte i = 0; i < num_cal_readings; i = i + 1) {
      x_cal_baseline_array[i] = analogRead(xpin);
      delay(20);
  }

  digitalWrite(ledPin, LOW);

  Serial.println("Collecting 1G data, hold x-axis  vertical");

  while (buttonState != HIGH){
    buttonState = digitalRead(buttonPin);
    delay(50);
  }

  digitalWrite(ledPin, HIGH);
  buttonState = LOW;

  for (byte i = 0; i < num_cal_readings; i = i + 1) {
        x_cal_1g_array[i] = analogRead(xpin);
        delay(20);
    }

  digitalWrite(ledPin, LOW);

  Serial.println("Done collecting data, now calibrating x-axis");

  float _1g;

  x_0 = average(x_cal_baseline_array);
  _1g = average(x_cal_1g_array);

  x_cal = abs(_1g - x_0);
  x_res = 1/x_cal;

  char buffer[80];
  sprintf(buffer, "This is the calibration value inside calibration: %d", x_cal);
  Serial.println(buffer);

  sprintf(buffer, "This is the resolution average inside calibration: %d", x_res);
  Serial.println(buffer);

  sprintf(buffer, "This is the baseline average inside calibration: %d", x_0);
  Serial.println(buffer);
  
}

float average(int arr[]){
  float result;
  int total = 0;
  for (byte i = 0; i < num_cal_readings; i = i + 1) {
    total = total + arr[i];
  }
  result = total / num_cal_readings;
  char buffer[80];
  sprintf(buffer, "This is the average value: %d", result);
  Serial.println(buffer);
  return result;
}