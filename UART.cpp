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

    // So the strings are seen as seperate strings
    hwlib::wait_ms(10);
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
        tx.set(getParityBit(data));

    }

    // Endbit(s)
    for(int i = 0; i < n_endbits; i++){
        baudPause();
        tx.set(true);
    }

    baudPause();
}

UART_READ sUART::read(int ms_limit) {
    int endtime = hwlib::now_us() + (1000 * ms_limit);

    int bitsReceived = 0;
    int endBitsReceived = 0;
    char packet = 0;

    UART_READ response = {};

    bool ledState = false;

    hwlib::target::pin_out led(hwlib::target::pins::d13);

    // Wait for the start bit
    while(getCurrentBit()){

        // Check to make sure we stop within the set time
        if(endtime < hwlib::now_us()){
            return UART_READ{0, UART_ERROR_CODES::TIME_EXCEEDED};
        }
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
            response.error = UART_ERROR_CODES::PARITY_ERROR;
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

    response.data = packet;

    return response;
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
    hwlib::wait_us_busy(104);
}

hwlib::string<100> sUART::readString(int ms_wait, int max_ms_time) {
    int max_endtime = -1;

    if(max_ms_time != -1){
        max_endtime = hwlib::now_us() + (1000 * max_ms_time);
    }

    bool continueReading = true;
    hwlib::string<100> data = "";

    while(continueReading && (hwlib::now_us() > max_endtime || max_endtime == -1)){
        UART_READ response = read(ms_wait);

        if(response.error == UART_ERROR_CODES::NONE){
            data += response.data;
        }else if(response.error == UART_ERROR_CODES::TIME_EXCEEDED){
            continueReading = false;
        }
    }

    return data;
}

hwlib::string<100> sUART::readUntil(char untilChar) {
    hwlib::string<100> data = "";

    UART_READ lastChar = read();
    data += lastChar.data;

    while(untilChar != lastChar.data){
        lastChar = read();
        data += lastChar.data;
    };

    return data;
}

hwlib::string<100> sUART::talk(hwlib::string<100> say) {
    outputString(say);
    return readString(1500);
}
