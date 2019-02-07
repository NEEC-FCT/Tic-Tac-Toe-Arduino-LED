int select_led_pin = 2;  // pin to switch leds
int confirm_play_pin = 3; // pin to confirm play and switch player

boolean allowed = 0; // if is the player's turn
char message; // message to send to other player

int state_button_1 = HIGH;      // the current state of the output pin
int reading_button_1;           // the current reading from the input pin
int previous_button_1 = LOW;    // the previous reading from the input pin

int state_button_2 = HIGH;      // the current state of the output pin
int reading_button_2;           // the current reading from the input pin
int previous_button_2 = LOW;    // the previous reading from the input pin

int counter_c = 0; // counter for colums
int counter_l = 0; // counter for lines

int columns[3] = {5, 6, 7}; // output pins for the matrix
int lines[3] = {8, 9, 10}; // output pins for the matrix

int no_led = 88;
int blue = 66;
int red = 82;
int color = blue;

int leds[3][3];

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long time = 0;         // the last time the output pin was toggled
long debounce = 200;   // the debounce time, increase if the output flickers

void Highlight_Led(int line, int column) {
  LedOn(line, column);
  delay(1);
  LedOff(line, column);
  delay(1);

}
void LedOn(int line, int column) {
  digitalWrite(lines[line], HIGH);
  digitalWrite(columns[column], LOW);
}
void LedOff(int line, int column) {
  digitalWrite(lines[line], LOW);
  digitalWrite(columns[column], HIGH);
}

void ClearLeds() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      LedOff(i, j);
    }
  }
}
void WriteLeds() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (leds[i][j] == color) {
        LedOn(i, j);
        delay(1);
        ClearLeds();
      }
    }
  }
}
void setup() {
  Serial.begin(9600);
  pinMode(select_led_pin, INPUT);
  pinMode(confirm_play_pin, INPUT);

  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++)
      leds[i][j] = no_led;
  }
  ClearLeds();
}

void loop() {
  WriteLeds();
  if (allowed) {
    reading_button_1 = digitalRead(select_led_pin);
    reading_button_2 = digitalRead(confirm_play_pin);

    if (reading_button_2 == HIGH && previous_button_2 == LOW && millis() - time > debounce) {
      leds[counter_l][counter_c] = color;
      message = 'A';
      allowed = 0;

      Serial.write(message);
      time = millis();
    }
    previous_button_2 = reading_button_2;
    // if the input just went from LOW and HIGH and we've waited long enough
    // to ignore any noise on the circuit, toggle the output pin and remember
    // the time
    if (reading_button_1 == HIGH && previous_button_1 == LOW && millis() - time > debounce) {
      counter_l++;
      if (counter_l == 3) {
        counter_l = 0;
        counter_c++;
      }
      if (counter_c == 3) {
        counter_c = 0;
      }
      time = millis();
    }
    Highlight_Led(counter_l, counter_c);

    previous_button_1 = reading_button_1;

  } else {
    if (Serial.available() > 0) {
      // read the incoming byte:
      message = Serial.read();
      if (message == 'A') {
        allowed = 1;
      }
    }
  }
}
