#include "aer.h"

// PINS
constexpr int num_in_data_pins = 3;
int in_data[num_in_data_pins] = {0, 1, 2};
constexpr int in_req = 3;
constexpr int in_ack = 4;

constexpr int num_out_data_pins = 5;
int out_data[num_out_data_pins] = {6, 7, 8};
constexpr int out_ack = 9;
constexpr int out_req = 10;

// buffers
uint8_t in_buf[256];

AER_in in_channel(in_req,
                  in_ack,
                  in_data,
                  num_in_data_pins,
                  in_buf);

AER_out out_channel(out_req,
                    out_ack,
                    out_data,
                    num_out_data_pins);

void in_channel_ISR() {
  if (!in_channel.reqRead())
  {
    if (in_channel.get_index() >= sizeof(in_buf)) { // buffer full
      usb_serial_write(in_buf, 256);
      in_channel.set_index(0);
    }
    in_channel.record_event();
    in_channel.ackWrite(0);
  }
  else
  {
    out_channel.ackWrite(1);
  }
}

// SETUP
void setup() {
  attachInterrupt(digitalPinToInterrupt(in_req), in_channel_ISR, CHANGE);
  sei(); // Enable interrupts
}

// MAIN LOOP
void loop() {  
  delay(1000); // wait 1 second

  cli(); // Disable Interrupts

  out_channel.dataWrite_handshake(0xF); // periodically send data over AER

  usb_serial_write(in_buf, in_channel.get_index()); // periodically send the data recieved over AER over USB
  in_channel.set_index(0);

  sei(); // Enable interrupts
}
