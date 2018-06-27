#ifndef JA_UART_HPP
#define JA_UART_HPP

#include "Globals.cpp"
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
        char read();
        bool getCurrentBit();
        hwlib::string<100> readUntil(char untilChar);

};
#endif
