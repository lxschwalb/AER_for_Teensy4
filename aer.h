#ifndef aer_h
#define aer_h

#include "Arduino.h"

class AER_in {
  public:
    AER_in(const int reqPin,
           const int ackPin,
           int dataPins[],
           int numDataPins,
           byte buff[] = NULL,
           int numTimestampBytes = 2,
           int d = 0,
           int timestampShift = 0,
           bool activeLow = true
    );
    bool reqRead();
    void ackWrite(bool val);

    unsigned int dataRead();
    unsigned int dataRead_handshake(); // TODO: check if this active low makes sense
    unsigned int record_event();
    unsigned int record_event_handshake(); // TODO: check if this active low makes sense

    void send_packet();
    int get_index();
    void set_index(int x);
    void set_t0(int t0);

  private:
    int _numDataPins;
    int _numTimestampBytes;
    int _numDataBytes;
    int _reqPin;
    int _ackPin;
    int* _dataPins;
    byte* _buff;
    int _index;
    int _t0;
    int _d;
    int _timestampShift;
    bool _activeLow;
};

class AER_out {
  public:
    AER_out(const int reqPin,
            const int ackPin,
            int dataPins[],
            int numDataPins,
            int d=0,
            bool activeLow = true
    );
    bool ackRead();
    void reqWrite(bool val);
    void dataWrite(unsigned int data);
    void dataWrite_handshake(unsigned int data);

  private:
    int _numDataPins;
    int _reqPin;
    int _ackPin;
    int* _dataPins;
    int _d;
    bool _activeLow;
};

#endif
