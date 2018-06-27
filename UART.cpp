//
// Created by Jordy Alkema on 26/06/18.
//

#include "UART.hpp"

void sUART::begin(int baudrate) {
    baudrate = baudrate;
    us_pause = (int)(1 / baudrate * 1000000);
}

bool sUART::getParityBit(char n){
    int count = 0;

    while (n)
    {
        count += n & 1;
        n >>= 1;
    }

    return (count % 2);
}

int sUART::getPacketSize() {
    return 1 + data_size + (int)include_parity + n_endbits;
}

void sUART::outputString(hwlib::string<100> string){
    for(uint8_t i = 0; i < string.length(); i++){
        outputChar(string[i]);
    }
}

void sUART::outputChar(char data){

    // Startbit
    tx.set(false);
    baudPause();

    // Databit(s)
    tx.set((data & 0x01));
    baudPause();
    tx.set((data & 0x02));
    baudPause();
    tx.set((data & 0x04));
    baudPause();
    tx.set((data & 0x08));
    baudPause();
    tx.set((data & 0x10));
    baudPause();
    tx.set((data & 0x20));
    baudPause();
    tx.set((data & 0x40));
    baudPause();
    tx.set((data & 0x80));


    // Paritybit
    if(include_parity){
        baudPause();
        tx.set(getParityBit());

    }

    // Endbit(s)
    for(int i = 0; i < n_endbits; i++){
        baudPause();
        tx.set(true);
    }

    // Set to HIGH
    tx.set(true);
}

char sUART::read() {
    int bitsReceived = 0;
    int endBitsReceived = 0;
    char packet = 0;

    bool ledState = false;

    hwlib::target::pin_out led(hwlib::target::pins::d13);

    // Wait for the start bit
    while(getCurrentBit()){
        led.set(true);
    }

    while(bitsReceived < data_size){
        led.set(false);

        baudPause();
        bool bit = getCurrentBit();

        packet = packet >> 1;

        if(bit){
            packet = (packet | 0x80);
        }

        bitsReceived++;
    }

    led.set(false);

    if(include_parity){
        baudPause();
        bool bit = rx.get();

        if(bit != getParityBit(packet)){
            //TODO: Error handler maken
        }
    }

    while(endBitsReceived < n_endbits){
        baudPause();
        bool bit = rx.get();

        if(!bit){
            continue;
        }

        endBitsReceived++;
    }

    return packet;
}

bool sUART::getCurrentBit(){
    bool firstPoll = rx.get();
    bool secondPoll = rx.get();
    bool thirdPoll = rx.get();

    if(firstPoll == secondPoll){
        return firstPoll;
    }

    if(secondPoll == thirdPoll){
        return secondPoll;
    }

    if(thirdPoll == firstPoll){
        return thirdPoll;
    }

    // If somehow the above does not work retry
    return getCurrentBit();
}

void sUART::baudPause(){
    hwlib::wait_us_busy(us_pause);
}

hwlib::string<100> sUART::readUntil(char untilChar) {
    hwlib::string<100> data = "";

    char lastChar = read();
    data += lastChar;

    while(untilChar != lastChar){
        lastChar = read();
        data += lastChar;
    };

    return data;
}