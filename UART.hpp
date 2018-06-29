#ifndef JA_UART_HPP
#define JA_UART_HPP

#include "Globals.cpp"
#include "DataTypes.cpp"
#include "libc-stub.cpp"

class sUART {
    private:

        ///////////////////////////
        ///      CONFIG         ///
        ///////////////////////////
        int data_size = 8;
        bool include_parity = false;
        bool n_endbits = 1;
        int baudrate = 9600;
        int us_pause;

        hwlib::target::pin_out tx;
        hwlib::target::pin_in rx;

    public:

        sUART(hwlib::target::pin_out tx, hwlib::target::pin_in rx):
                tx(tx), rx(rx){
                    // UART default(inactive) is HIGH
                    tx.set(true);
        };

        void begin(int baudrate);
        bool getParityBit(char Data);
        int getPacketSize();
        void baudPause();

        // WRITE
        void outputChar(char data);
        void outputString(hwlib::string<100> data);

        // READ
        UART_READ read(int ms_limit = 10000);
        bool getCurrentBit();
        hwlib::string<100> readString(int ms_wait = 1000, int max_ms_time = -1);
        hwlib::string<100> readUntil(char untilChar);

        // COMBO
        hwlib::string<100> talk(hwlib::string<100> say);

};

#include "UART.cpp"

#endif
