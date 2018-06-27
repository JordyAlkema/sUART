//
// Created by Jordy Alkema on 26/06/18.
//

#include "UART.hpp"

bool sUART::print(hwlib::string<100> data) {
    DataPackets dataPackets = createDataPackets(data);

    for(int i = 0; i < dataPackets.n_packets; i++){
        sendPacket(dataPackets.packets[i]);
    }

    return true;
}

void sUART::sendPackets(DataPackets packets) {
    for(int i = 0; i < packets.n_packets; i++){
        sendPacket(packets.packets[i]);
    }
}

void sUART::sendPacket(char16_t dataPacket) {

    // remove unused bits to save time.
   // dataPacket = dataPacket >> (16 - getPacketSize());

    for(int i = 0; i < getPacketSize(); i++){
        bool value = (dataPacket & 1);
        dataPacket = dataPacket >> 1;

        tx.set(value);

        hwlib::wait_us_busy(104);
    }

    tx.set(true);
}


void sUART::begin(int baudrate) {
    baudrate = baudrate;
    us_pause = (int)(getPacketSize() / baudrate * 1000000);
}

DataPackets sUART::createDataPackets(hwlib::string<100> data) {

    char16_t packets[data.nsize] = {};

    for(uint8_t i = 0; i < data.nsize; i++){
        char current_character = data[i];
        char16_t dataPacket = 0;

        for(int i = 0; i < n_endbits; i++) {
            dataPacket = dataPacket << 1; // Make room for end bits
            //dataPacket = dataPacket | 1; // End bits
        }

        if(include_parity) {
            dataPacket = dataPacket << 1; // Make room for Parity bit
            dataPacket = dataPacket | getParityBit(current_character);
        }


        dataPacket = dataPacket << 8;
        //dataPacket = (dataPacket | current_character);
        // Add the data LSB->MSB so the RX gets it MSB->LSB
        //bool test = false;
        //for(int i = 0; i < 8; i++){
        //    dataPacket = dataPacket << 1; // Make room for data bit
            //dataPacket = dataPacket | test; //(current_character & 1); // Add bit
           // current_character = current_character >> 1; // move bits so we can get the next bit
            //test = !test;
        //}


        dataPacket = dataPacket << 1; // Add start bit

        packets[i] = dataPacket;
    }

    DataPackets dataPackets = {packets, data.nsize};

    return dataPackets;
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

void sUART::test(){
//    for(int i = 0; i < 5; i++){
//        tx.set(false);
//        hwlib::wait_ms(100);
//        tx.set(true);
//        hwlib::wait_ms(100);
//    }

    tx.set(false);
    hwlib::wait_us_busy(104);
    tx.set(false);
    hwlib::wait_us_busy(104);
    tx.set(true);
    hwlib::wait_us_busy(104);
    tx.set(false);
    hwlib::wait_us_busy(104);
    tx.set(false);
    hwlib::wait_us_busy(104);
    tx.set(true);
    hwlib::wait_us_busy(104);
    tx.set(true);
    hwlib::wait_us_busy(104);
    tx.set(true);
    hwlib::wait_us_busy(104);
    tx.set(true);
    hwlib::wait_us_busy(104);
    tx.set(true);
    hwlib::wait_us_busy(104);

    tx.set(true);

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

    //TODO: Net maken
    tx.set(false);
    hwlib::wait_us_busy(104);
    tx.set((data & 0x01));
    hwlib::wait_us_busy(104);
    tx.set((data & 0x02));
    hwlib::wait_us_busy(104);
    tx.set((data & 0x04));
    hwlib::wait_us_busy(104);
    tx.set((data & 0x08));
    hwlib::wait_us_busy(104);
    tx.set((data & 0x10));
    hwlib::wait_us_busy(104);
    tx.set((data & 0x20));
    hwlib::wait_us_busy(104);
    tx.set((data & 0x40));
    hwlib::wait_us_busy(104);
    tx.set((data & 0x80));
    hwlib::wait_us_busy(104);
    tx.set(true);
    hwlib::wait_us_busy(104);
    tx.set(true);
}

char sUART::read() {
    int bitsReceived = 0;
    int endBitsReceived = 0;
    char packet = 0;

    bool ledState = false;

    hwlib::target::pin_out led(hwlib::target::pins::d13);

    while(getCurrentBit()){
        led.set(true);
        //hwlib::wait_us_busy(104);
    }

    while(bitsReceived < data_size){
        led.set(false);

        hwlib::wait_us_busy(104);
        bool bit = getCurrentBit();

        packet = packet >> 1;

        if(bit){
            packet = (packet | 0x80);
        }

        bitsReceived++;
    }

    led.set(false);

    if(include_parity){
        hwlib::wait_us_busy(104);
        bool bit = rx.get();

        if(bit != getParityBit(packet)){
            //TODO: Error handler maken
        }
    }

    while(endBitsReceived < n_endbits){
        hwlib::wait_us_busy(104);
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

    return getCurrentBit();
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