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

#define RF_CE_PIN 9
#define RF_CSN_PIN 8
#define RF_IRQ_PIN 2


static RF24 radio(RF_CE_PIN, RF_CSN_PIN);

static void set_controller_status(byte status);
static void handle_command();

static byte _current_status = STATUS_UNKNOWN;

void setup()
{
  Serial.begin(115200);
  delay(5000);
  Serial.println(F("Vattenfel controller unit booting (build " __DATE__ " " __TIME__ ")"));

  radio.begin();
  radio.openWritingPipe(panel_address);
  radio.openReadingPipe(1, controller_address);
  radio.setPayloadSize(1);
  radio.setAutoAck(true);
  radio.enableAckPayload();
  radio.setDataRate(RF24_250KBPS);

  set_controller_status(STATUS_WATER_ON);
  radio.startListening();
}

void loop()
{
  while (radio.available()) {
    handle_command();
  }
}


void set_controller_status(byte status)
{
  if (status == _current_status) {
    return;
  }

  _current_status = status;

  byte buf[1];
  buf[0] = status;

  radio.flush_tx();
  radio.writeAckPayload(1, buf, 1);
}


void handle_command()
{
  byte buf[1];
  char command;

  radio.read(buf, 1);
  command = buf[0];

  // Refresh ack
  buf[0] = _current_status;
  radio.writeAckPayload(1, buf, 1);

  Serial.print(F("Got command from panel: "));
  Serial.println(command);
}
