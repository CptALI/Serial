#include <Windows.h>
#include <tchar.h>
#include <iostream>
#include <string>

#define COM "COM8"

HANDLE hSerial;


// Function to initialize COM port
bool COMInit(const TCHAR* portName) {
    hSerial = CreateFile(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Error: Unable to open COM port\n";
        return false;
    }

    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error: Unable to get COM port state\n";
        CloseHandle(hSerial);
        return false;
    }
    dcbSerialParams.BaudRate = CBR_38400;  // Set your desired baud rate here
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = 0; // Set stop bits to 0
    dcbSerialParams.Parity = NOPARITY;
    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error: Unable to set COM port state\n";
        CloseHandle(hSerial);
        return false;
    }

    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    if (!SetCommTimeouts(hSerial, &timeouts)) {
        std::cerr << "Error: Unable to set COM port timeouts\n";
        CloseHandle(hSerial);
        return false;
    }

    return true;
}

// Function to close COM port
void COMEnd() {
    if (hSerial != INVALID_HANDLE_VALUE) {
        CloseHandle(hSerial);
        hSerial = INVALID_HANDLE_VALUE;
    }
}

// Function to send data to COM port
bool COMSend(const std::string& data) {
    DWORD bytesWritten;
    if (!WriteFile(hSerial, data.c_str(), data.length(), &bytesWritten, NULL)) {
        std::cerr << "Error: Unable to write to COM port\n";
        return false;
    }
    return true;
}

// Function to read data from COM port
std::string COMRead() {
    const int BUFFER_SIZE = 128;
    char buffer[BUFFER_SIZE];
    DWORD bytesRead;
    if (!ReadFile(hSerial, buffer, BUFFER_SIZE, &bytesRead, NULL)) {
        std::cerr << "Error: Unable to read from COM port\n";
        return "";
    }
    return std::string(buffer, bytesRead);
}

bool BTInit() {
    std::string command1 = "AT\r\n";
    std::string command2 = "AT+INQ\r\n";
    std::string command3 = "AT+CONN1\r\n";
    std::string answer1;
    std::string answer2;
    //std::string answer2_sub;
    std::string answer3;
    //std::string answer3_sub;
    bool condition1 = false;
    bool condition2 = false;
    bool condition3 = false;

    //////////////////////////////////////////////////////////////////////////
    Sleep(50);
    if (COMSend(command1)) {
        std::cout << "AT Checking!\n";
    }
    else {
        std::cout << "Failed to send data to master\n";
        return false;
    }
    Sleep(50);
    answer1 = COMRead();
    if (!answer1.empty()) {
        std::cout << "Received Response\n" << answer1;
    }
    else {
        std::cout << "Master module is not responding\n";
        return false;
    }
    if (answer1.find("OK") != std::string::npos) {
        condition1 = true;
    }
    else {
        std::cout << "There is an error in the first initiation command\n";
        return false;
    }
    /////////////////////////////////////////////////////////////////////////////
    Sleep(50);
    if (COMSend(command2)) {
        std::cout << "AT Inquiring slave devices!\n";
    }
    else {
        std::cout << "Failed to send data to master\n";
        return false;
    }
    Sleep(50);
    answer2 = COMRead();
    if (!answer2.empty()) {
        std::cout << "Received Response\n" << answer2;
    }
    else {
        std::cout << "Master module is not responding\n";
        return false;
    }
    Sleep(500);
    if (answer2.find("1") != std::string::npos ) {
        condition2 = true;
    }
    else {
        std::cout << "There is an error in the seccond initiation command\n";
        return false;
    }
    /////////////////////////////////////////////////////////////////////////////////////////
    Sleep(50);
    if (COMSend(command3)) {
        std::cout << "AT connecting to the device number1 !\n";
    }
    else {
        std::cout << "Failed to send data to master\n";
        return false;
    }
    Sleep(500);
    answer3 = COMRead();
    if (!answer3.empty()) {
        std::cout << "Received Response\n" << answer3;
    }
    else {
        std::cout << "Master module is not responding\n";
        return false;
    }
    if ((answer3.find("CONNECTED") != std::string::npos)||(answer3.find("Connected") != std::string::npos)) {
        condition3 = true;
    }
    else {
        std::cout << "There is an error in the third initiation command\n";
        return false;
    }
    std::cout << "BT SUCCESSFULLY INITIATED\n";
    Sleep(2000);
    return condition1 && condition2 && condition3;

}

int main() {
    const TCHAR* portName = _T(COM);
    char sendingBytes[4];
    sendingBytes[0] = 16;
    sendingBytes[1] = '\r';
    sendingBytes[2] = '\n';
    sendingBytes[3] = '\0';


    // Initialize COM port
    if (COMInit(portName)) {
        std::cout << "COM port opened successfully!\n";

        std::cout << "Is the bluetooth initiated?(Y/N)";
        char bt_init = getchar();
        if ((bt_init == 'N') || (bt_init == 'n')) {
            if (BTInit()) {
                //Sleep(1000);
                for (int counter = 16; counter < 254; counter++) {
                    sendingBytes[0] = counter;
                    COMSend(sendingBytes);
                    Sleep(10);
                }
                for (int counter = 254; counter > 16; counter--) {
                    sendingBytes[0] = counter;
                    COMSend(sendingBytes);
                    Sleep(10);
                }

            }
        }
        else {
            //Sleep(1000);
            for (int counter = 16; counter < 254; counter++) {
                sendingBytes[0] = counter;
                COMSend(sendingBytes);
                Sleep(10);
            }
            for (int counter = 254; counter > 16; counter--) {
                sendingBytes[0] = counter;
                COMSend(sendingBytes);
                Sleep(10);
            }
        };

        // Close COM port
        COMEnd();
        std::cout << "COM port closed successfully!\n";
    }
    else {
        std::cerr << "Failed to open COM port\n";
        return 1;
    }

    return 0;
}
