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

    for(int i = 0; i < packet_size; i++){
        bool value = dataPacket & 1;
        dataPacket = dataPacket >> 1;

        tx.set(value);

        hwlib::wait_us(us_pause);
    }

    tx.set(true);
}


void sUART::begin(int baudrate) {
    baudrate = baudrate;
    us_pause = (int)(packet_size / baudrate * 100000);
}

DataPackets sUART::createDataPackets(hwlib::string<100> data) {

    char16_t packets[data.nsize] = {};

    for(uint8_t i = 0; i < data.nsize; i++){
        char current_character = data[i];
        char16_t dataPacket = 0;

        dataPacket = dataPacket << 1; // Add start bit
        dataPacket = dataPacket << 8; // Make room for data bits
        dataPacket = dataPacket | current_character; // Add data
        dataPacket = dataPacket << 1; // Make room for Parity bit
        dataPacket = dataPacket | getParityBit(current_character);
        dataPacket = dataPacket << 2; // Make room for end bits
        dataPacket = dataPacket | 3; // End bits

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
    for(int i = 0; i < 5; i++){
        tx.set(false);
        hwlib::wait_ms(100);
        tx.set(true);
        hwlib::wait_ms(100);
    }
}