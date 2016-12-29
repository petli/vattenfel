// -*-c++-*-

#include <RF24.h>

static byte panel_address[] = "VFPAN";
static byte controller_address[] = "VFCTR";

#define STATUS_UNKNOWN '-'
#define STATUS_WATER_ON 'n'
#define STATUS_WATER_OFF 'f'
#define STATUS_PRESSURE_LOW '!'

#define CMD_STATUS '?'
#define CMD_WATER_ON 'N'
#define CMD_WATER_OFF 'F'

#define RF_CE_PIN 8
#define RF_CSN_PIN 11

static RF24 radio(RF_CE_PIN, RF_CSN_PIN);

static void get_controller_status();

void setup()
{
  Serial.begin(115200);
  delay(5000);
  Serial.println(F("Vattenfel control panel booting (build " __DATE__ " " __TIME__ ")"));

  radio.begin();
  radio.openWritingPipe(controller_address);
  radio.openReadingPipe(1, panel_address);
  radio.setPayloadSize(1);
  radio.setAutoAck(true);
  radio.enableAckPayload();
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();
}

void loop()
{
  get_controller_status();
  delay(2000);
}


void get_controller_status()
{
  byte buf[1];

  buf[0] = CMD_STATUS;

  if (!radio.write(buf, 1)) {
    Serial.println(F("Failed to send command to controller"));
    return;
  }

  if (!radio.available()) {
    Serial.println(F("No status returned from controller"));
    return;
  }

  char status = STATUS_UNKNOWN;
  while (radio.available()) {
    radio.read(buf, 1);
    status = buf[0];
  }

  Serial.print(F("Controller status: "));
  Serial.println(status);
}
