#ifndef JA_UART_HPP
#define JA_UART_HPP

#include "DataTypes.cpp"
#include "libc-stub.cpp"

class sUART {
    private:

        ///////////////////////////
        ///      CONFIG         ///
        ///////////////////////////
        int data_size = 8;
        bool include_parity = false;
        bool n_endbits = 0;
        int baudrate = 9600;
        int us_pause;

        hwlib::target::pin_out tx;
        hwlib::target::pin_in rx;

    public:

    sUART(hwlib::target::pin_out tx, hwlib::target::pin_in rx):
            tx(tx), rx(rx){};

        void begin(int baudrate);
        DataPackets createDataPackets(hwlib::string<100> data);
        void sendPackets(DataPackets packets);
        bool getParityBit(char Data);
        void sendPacket(char16_t dataPacket);
        int getPacketSize();
        void test();
        bool getBit();
        bool print(hwlib::string<100> data);
        char read();
};
#endif
