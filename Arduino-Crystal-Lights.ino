// This is a simple light project for the Arduino. Basically my partner bought some
// crystals with LEDs in them (https://www.amazon.com/Catstar-Fluorite-Operated-Christmas-Ornaments/dp/B07SQ3L1ZD)
// and they wouldn't stay on continuously, instead they would pulse in/out at an
// unpleasant rate.
//
// So I took on the project of disconnecting the LEDs from the control box that
// they came with and connecting them to an Arduino. I was able to have the Arduino
// perform two actions with them:
//  * Have all lights continuously on
//  * Fade half of the lights in/out
// 
// I was unable to have all of the lights fade at the same time. Interestingly,
// the lights only had 2 strings, which I had guessed was positive/negative, but
// after experimenting, I learned that if I provided power from only one side,
// it would turn on half of the lights. If I then provided power from the other
// side it would turn on the other half of the lrights.
//
// This posed a rather interesting problem of only being able to power half of
// the lights at a time. However, I learned through experimentation that if I
// powered half of them at full, then switch to powering the other half at full
// and repeated this, it would actually light all of the lights!
//
// I also experimented with adding a button control to switch between modes, but
// that code is currently commented out as my partner only cares about having all
// the lights on at the same time. I've left the code in for future work if we
// do decide to upgrade the hardware and add a button.

// When testing with an Uno/Mega, I used the following pins
// #define LED_PIN_1 2
// #define LED_PIN_2 3

// When moving to the Nano, I used the following pins:
#define LED_PIN_1 A3
#define LED_PIN_2 A4

// #define BUTTON_PIN 4

// Various defines for controlling the current operational mode of
// the lights.
#define STATE_STRAND_1            0
#define STATE_STRAND_2            1

#define STATE_STRAND_1_FADE_IN    0
#define STATE_STRAND_1_FADE_OUT   1
#define STATE_STRAND_2_FADE_IN    3
#define STATE_STRAND_2_FADE_OUT   4

#define MODE_CONSTANT             0
#define MODE_SLOW_FADE            1
#define MODE_FAST_FADE            2

// Controls for adjusting the fade lighting, allowing it to fade
// in/out quicker/slower.
#define FADE_AMOUNT               1

// To retain the current mode/state of the LEDs, I create these
// as global variables to allow for keeping the state between
// calls to loop()
int brightness = 0;
int state = 0;
int mode = MODE_CONSTANT;
// int buttonState = 0;

// the setup function runs once when you press reset or power the board
void setup() 
{
  // Set both of our pins as output pins for our board to control
  // the lrights. While I'm not an expert at this, I'm guessing that
  // when one pin is LOW and the other is HIGH, the LOW pin is acting
  // as a ground.
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  // pinMode(BUTTON_PIN, INPUT);
}


// the loop function runs over and over again forever
void loop() {
//  buttonState = digitalRead(BUTTON_PIN);
//  if (buttonState == HIGH)
//  {
//    switch(mode)
//    {
//      case MODE_CONSTANT:
//        mode = MODE_SLOW_FADE;
//        break;
//      case MODE_SLOW_FADE:
//        mode = MODE_FAST_FADE;
//        break;
//      case MODE_FAST_FADE:
//        mode = MODE_CONSTANT;
//        break;
//    }
//  }

  // Depending on what mode is selected for the lights, we'll
  // go to the corresponding function call to set the lights
  // correctly.
  switch(mode)
  {
    case MODE_CONSTANT:
      AllConstant();
    break;
    case MODE_SLOW_FADE:
      FadeInOut(100);
    break;
    case MODE_FAST_FADE:
      FadeInOut(10);
    break;
  }
}

// This function alternates between which light strand is set
// to high/low each time it's called, to allow the strand to
// appear as if all lights are on at the same time.
void AllConstant()
{
  switch(state)
  {
    case STATE_STRAND_1:
      digitalWrite(LED_PIN_1, HIGH);
      digitalWrite(LED_PIN_2, LOW);
      state = STATE_STRAND_2;
    break;
    case STATE_STRAND_2:
      digitalWrite(LED_PIN_1, LOW);
      digitalWrite(LED_PIN_2, HIGH);
      state = STATE_STRAND_1;
    break;
  }
}

// This function will take the lights and slowly fade in/out
// of one strand, the switch to the second strand and fade
// in/out, then go back to the original strand. The speed at
// which the fade happens is controlled by the argument `d`
// allowing for a faster or slower fade to happen.
void FadeInOut(int d)
{
  switch(state)
  {
    case STATE_STRAND_1_FADE_IN:
      brightness += FADE_AMOUNT;
      
      if (brightness > 255)
      {
        brightness = 255;
        state = STATE_STRAND_1_FADE_OUT;
      }
      
      analogWrite(LED_PIN_1, brightness);
      analogWrite(LED_PIN_2, 0);
    break;
    case STATE_STRAND_1_FADE_OUT:
      brightness -= FADE_AMOUNT;
      
      if (brightness < 0)
      {
        brightness = 0;
        state = STATE_STRAND_2_FADE_IN;
      }
      
      analogWrite(LED_PIN_1, brightness);
      analogWrite(LED_PIN_2, 0);
    break;
    case STATE_STRAND_2_FADE_IN:
      brightness += FADE_AMOUNT;
      
      if (brightness > 255)
      {
        brightness = 255;
        state = STATE_STRAND_2_FADE_OUT;
      }
      
      analogWrite(LED_PIN_1, 0);
      analogWrite(LED_PIN_2, brightness);
    break;
    case STATE_STRAND_2_FADE_OUT:
      brightness -= FADE_AMOUNT;
      
      if (brightness < 0)
      {
        brightness = 0;
        state = STATE_STRAND_1_FADE_IN;
      }
      
      analogWrite(LED_PIN_1, 0);
      analogWrite(LED_PIN_2, brightness);
    break;
  }
  delay(d);
}
