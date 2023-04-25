//
//  FF_WATCHDOG: implements a watchdog using an ATtiny85
//  
//  This code resets an external controller when not receiving trigger pulses for 20s
//    (30s after this MCU boot or after external MCU reset)
//
//             +-----+
//      N/C 1 -+ O   +- 8 VCC (3 to 5V)
//      N/C 2 -+     +- 7 N/C
//      N/C 3 -+     +- 6 Trigger (from external MCU)
//      GND 4 -+     +- 5 Reset (to external MCU through a 470 Ohms resistor)
//             +-----+
//
//  When programming ATtiny with an Arduino, connections should be:
//    Arduino - ATtiny
//      D10   -   1
//      D11   -   5
//      D12   -   6
//      D12   -   7
//      GND   -   4
//       5V   -   8
//
//  Memory/Flash usage:
//      Sketch uses 1146 bytes (13%) of program storage space. Maximum is 8192 bytes.
//      Global variables use 21 bytes (4%) of dynamic memory, 
//          leaving 491 bytes for local variables. Maximum is 512 bytes.
//
//  Written and maintained by Flying Domotic (https://github.com/FlyingDomotic)
//

// Pin connected to reset of external uController (0 -> chip pin #5)
#define RESET_PIN 0

// Pin to receive trigger from external uController (1 -> chip pin #6)
#define TRIGGER_PIN 1

// To set ideas, downoading an ESP8266 prog over Serial or OTA is around 15 secs,
//  and boot time around 3 to 5 secs. Setting "normal" time-out to 30 secs makes it.

// WatchDog time out (in ms) when in normal mode
#define TIME_OUT_RUN 30000

// WatchDog time out (in ms) when we (re)start uController
#define TIME_OUT_START 40000

// Last time trigger PIN changed
unsigned long lastTriggerTime = 0;

// Current time-out value
unsigned long currentTimeout = TIME_OUT_START;

// Last state when trigger PIN changed
int lastTriggerState = false;

// Current trigger PIN state
int currentState = true;

void setup() {
  // Set the 2 pins on input mode (Reset PIN will be floating)
  pinMode(RESET_PIN, INPUT);
  pinMode(TRIGGER_PIN, INPUT);

  // Next timeout will be a start one
  currentState = digitalRead(TRIGGER_PIN);
  currentTimeout = TIME_OUT_START;
  
  // Save startup time
  lastTriggerTime = millis();
}

void loop() {
  // Check trigger pin
  currentState = digitalRead(TRIGGER_PIN);
  
  // Reset timer if trigger state changed
  if (currentState != lastTriggerState) {
    // Save last trigger time
    lastTriggerTime = millis();
    // Save last trigger state
    lastTriggerState = currentState;
    // Set timeout to run value
    currentTimeout = TIME_OUT_RUN;
  }
  
  // Reset external uController if last trigger is too old
  if ((millis() - lastTriggerTime) > currentTimeout) {
    // Set reset pin low for 500 ms
    digitalWrite(RESET_PIN, LOW);
    pinMode(RESET_PIN, OUTPUT);
    delay(500);
    // Set pin HIGH
    digitalWrite(RESET_PIN, HIGH);
    // Set input mode to disconnect PIN
    pinMode(RESET_PIN, INPUT);
  
    // Reset last trigger time
    lastTriggerTime = millis();
    
    // Next timeout will be a start one (as we just reset uC)
    currentTimeout = TIME_OUT_START;
  }
}
