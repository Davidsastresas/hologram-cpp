#include "Modems\Nova.h"
#include <iostream>

int main()
{
	//Modem::getConnectionProfiles();
		Nova nova;
		std::string input = "";
		for (auto modem : Modem::getConnectedModems()) {
			std::wcout << "Found Modem -> " << modem->szDeviceName << std::endl;
			std::cout << "Connect? ";
			std::getline(std::cin, input);
			std::transform(input.begin(), input.end(), input.begin(), ::tolower);
			if (input == "yes" || input == "y") {
				std::cout << "Name the connection: ";
				std::getline(std::cin, input);
				std::wstring wstr;
				StringToWstring(CP_ACP, input, wstr);
				nova.setupCellularDataConnection(modem->szDeviceName, wstr);
				break;
			}
		}
		
		for (auto device : Serial::getConnectedSerialDevices()) {
			std::wcout << "Found Serial Device " << device.friendlyName.c_str() << std::endl;
			std::cout << "Connect? ";
			std::getline(std::cin, input);
			std::transform(input.begin(), input.end(), input.begin(), ::tolower);
			if (input == "yes" || input == "y") {
				if (nova.setupSerialPort(device.portName, 9600)) {
					break;
				}
				else {
					std::cout << "Failed to connect to port..." << std::endl;
				}
			}
		}

		std::vector<std::string> resultArray;
		input = "";
		std::cout << "Type Connect/Disconnect or an AT command " << std::endl;
		while (input != "exit") {
			std::cout << "Command? ";
			std::getline(std::cin, input);
			if (input == "connect") {
				if (nova.connect()) {
					std::cout << "Connected" << std::endl;
				}
				else {
					std::cout << "Failed to Connect" << std::endl;
				}
			}
			else if (input == "disconnect") {
				nova.disconnect();
			}
			else {
				if (nova.sendAndParseATCommand(input, resultArray)) {
					for (auto result : resultArray) {
						std::cout << "Result: " << result << std::endl;
					}
				}
				else {
					std::cout << "Sending AT Command Failed" << std::endl;
					for (auto result : resultArray) {
						std::cout << "Result: " << result << std::endl;
					}
				}
				resultArray.clear();
			}
		}
		nova.disconnect();
	return 0;
}