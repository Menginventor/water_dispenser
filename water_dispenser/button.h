//Debouncing method from https://www.arduino.cc/en/tutorial/debounce
class Button {
  private:
    byte pressedState;
  public:
    byte btn_pin;

    unsigned lastDebounceTime;
    unsigned long debounceDelay = 50;
    int buttonState;
    int lastButtonState;

    bool pressed_flag = false;
    bool released_flag = false;
    bool toggled_flag = false;

    Button(byte pin, byte mode, byte _pressedState) {
      btn_pin = pin;
      pressedState = _pressedState;
      pinMode(pin, mode);
      buttonState = digitalRead(btn_pin);
      lastButtonState = digitalRead(btn_pin);
      lastDebounceTime = millis();
    }
    void update() {
      pressed_flag = false;
      released_flag = false;
      toggled_flag = false;
      //
      int reading = digitalRead(btn_pin);


      if (reading != lastButtonState) {
        // reset the debouncing timer
        lastDebounceTime = millis();
      }

      if ((millis() - lastDebounceTime) > debounceDelay) {
        // whatever the reading is at, it's been there for longer than the debounce
        // delay, so take it as the actual current state:

        // if the button state has changed:
        if (reading != buttonState) {
          buttonState = reading;
          toggled_flag = true;
          if (reading == pressedState) {
            pressed_flag = true;

          }
          else {
            released_flag = true;
          }
        }
      }
      lastButtonState = reading;
    }
    bool read() {
      return buttonState;
    }
    bool pressed() {
      return pressed_flag;
    }
    bool released() {
      return released_flag;
    }
    bool toggled() {
      return toggled_flag;
    }
};
