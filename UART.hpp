#ifndef JA_UART_HPP
#define JA_UART_HPP

#include "DataTypes.cpp"
#include "libc-stub.cpp"

/// s(oftware)UART class
class sUART {
    private:

        /// The data size of each packet
        int data_size = 8;

        /// if a packet has an parity bit.
        bool include_parity = false;

        /// The amount of end bits.
        bool n_endbits = 1;

        /// The baud rate
        int baudrate = 9600;

        /// The calculated pause between bits in us(nanoseconds)
        int us_pause;

        /// The transmission pin
        hwlib::target::pin_out tx;

        /// The read pin
        hwlib::target::pin_in rx;

    public:

        sUART(hwlib::target::pin_out tx, hwlib::target::pin_in rx):
                tx(tx), rx(rx){
                    // UART default(inactive) is HIGH
                    tx.set(true);
        };

        /// sets the baudrate for the communication and calculates the pause between bits.
        /// @param baudrate the baudrate for the communication
        void begin(int baudrate);

        /// Calculates the Parity bit
        /// @param Data the bit to calculate the Parity for
        /// @returns Bool
        bool getParityBit(char Data);

        /// Returns the total packet size.
        /// @returns int
        int getPacketSize();

        /// Pauses transmission for the exact duration needed at the specified baudrate
        void baudPause();

        /// sends an single byte of data
        /// @param data the byte to send
        void outputChar(char data);

        /// Sends an string of data
        /// @param data string to send
        void outputString(hwlib::string<100> data);

        /// Reads an byte
        /// @param ms_limit how long it should wait before returning without data.
        /// @returns UART_READ an struct that has the Data Byte and an errorCode if something went wrong
        UART_READ read(int ms_limit = 10000);

        /// Reads an bit
        /// @returns the bit
        bool getCurrentBit();

        /// combines multiple reads into an string
        /// @param ms_wait maximum time before it will be assumed it is the end of the string
        /// @param max_ms_time optional maximum time the execution may take
        /// @returns String
        hwlib::string<100> readString(int ms_wait = 1000, int max_ms_time = -1);

        /// Reads data until an character is found, and returns all characters read before.
        /// @param untilChar Break character
        /// @returns String
        hwlib::string<100> readUntil(char untilChar);

        /// Outputs data and waits for an response.
        /// @param string to send
        /// @returns string that has been send back
        hwlib::string<100> talk(hwlib::string<100> say);

};

#include "UART.cpp"

#endif
