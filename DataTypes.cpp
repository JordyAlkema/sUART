/// All possible error codes
enum UART_ERROR_CODES: int{
    NONE = 0,
    PARITY_ERROR = 1,
    TIME_EXCEEDED = 2
};

/// Struct used for retuning an byte of data and an optional error
struct UART_READ{
    char data = 0;
    UART_ERROR_CODES error = NONE;

};