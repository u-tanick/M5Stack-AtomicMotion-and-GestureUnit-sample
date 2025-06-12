#include <M5Unified.h>
#include <Wire.h>
#include <DFRobot_PAJ7620U2.h>
#include <M5AtomicMotion.h>

DFRobot_PAJ7620U2 PAJ7620U2;
M5AtomicMotion AtomicMotion;

// AtomS3 / AtomS3-Lite Grove Port.A
#define SDA_PAJ7620U2 2
#define SCL_PAJ7620U2 1

// Atomic Motion I2C PIN
#define SDA_ATOM_MOTION 38
#define SCL_ATOM_MOTION 39

#define USE_I2C_SCANNER

void setup()
{
  auto cfg = M5.config();
  M5.begin(cfg);

  Serial.begin(115200);
  delay(300);

  // ------------------------------------------------------
  // Setting Wire for PAJ7620U2
  Serial.println("Wire I2C Port.A SDA: 2, SCL: 1");
  Wire.begin(SDA_PAJ7620U2, SCL_PAJ7620U2);

#ifdef USE_I2C_SCANNER
  Serial.println("I2C Scanner 1");
  for (byte address = 1; address < 127; ++address)
  {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0)
    {
      Serial.print("Found I2C device at 0x");
      Serial.println(address, HEX);
    }
  }
#endif

  // ------------------------------------------------------
  // Init PAJ7620U2
  Serial.println("Init PAJ7620U2");
  while (PAJ7620U2.begin() != 0)
  {
    Serial.println("PAJ7620U2 init failed");
    delay(500);
  }
  Serial.println("PAJ7620U2 init completed");
  PAJ7620U2.setGestureHighRate(false);

  // ------------------------------------------------------
  // Setting Wire1 for AtomicMotion
  Serial.println("Wire AtomS3 backside PIN SDA: 38, SCL: 39");
  Wire1.begin(SDA_ATOM_MOTION, SCL_ATOM_MOTION);

#ifdef USE_I2C_SCANNER
  Serial.println("I2C Scanner 2");
  for (byte address = 1; address < 127; ++address)
  {
    Wire1.beginTransmission(address);
    if (Wire1.endTransmission() == 0)
    {
      Serial.print("Found I2C device at 0x");
      Serial.println(address, HEX);
    }
  }
#endif

  // ------------------------------------------------------
  // Init AtomicMotion
  Serial.println("Init AtomicMotion");
  while (
      !AtomicMotion.begin(&Wire1, M5_ATOMIC_MOTION_I2C_ADDR, SDA_ATOM_MOTION, SCL_ATOM_MOTION, 100000))
  {
    Serial.println("Atomic Motion init failed");
    delay(1000);
  }
  Serial.println("Atomic Motion init completed");
}

void loop()
{
  DFRobot_PAJ7620U2::eGesture_t gesture = PAJ7620U2.getGesture();
  if (gesture != PAJ7620U2.eGestureNone)
  {
    /* Get the string descritpion corresponding to the gesture number
     * The string description could be
     * "None","Right","Left", "Up", "Down", "Forward", "Backward", "Clockwise", "Anti-Clockwise", "Wave",
     * if setGestureHighRate is true, these gestures enabled.
     * "WaveSlowlyDisorder", "WaveSlowlyLeftRight", "WaveSlowlyUpDown", "WaveSlowlyForwardBackward"
     */
    String description = PAJ7620U2.gestureDescription(gesture);
    Serial.println("--------------Gesture Recognition System---------------------------");
    Serial.print("gesture code        = ");
    Serial.println(gesture);
    Serial.print("gesture description  = ");
    Serial.println(description);
    Serial.println();

    String ges = "Right";

    if (ges == description)
    {
      AtomicMotion.setServoAngle(0, 180);
      delay(300);
      AtomicMotion.setServoAngle(0, 0);
    }
  }
}
