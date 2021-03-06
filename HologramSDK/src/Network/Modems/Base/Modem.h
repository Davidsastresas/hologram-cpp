#pragma once
#include "Serial/Serial.h"
#include "IP/ConnectionManager.h"
#include "../../../Event/EventBus/EventBus.h"
#include "../../../Event/Events.h"

#include <deque>

#define RETRY_DELAY 50  // 50 millisecond delay to avoid spinning loops

typedef struct _MODEM_INFO {
	std::string Manufacturer;
	std::string Model;
	std::string Revision;
	std::string SVN;
	std::string IMEI;
	std::string ICCID;
	std::string Mode;
}MODEM_INFO;

enum ModemResult {
	MODEM_OK,
	MODEM_TIMEOUT,
	MODEM_INVALID,
	MODEM_NO_MATCH,
	MODEM_ERROR
};

enum URCState {
	SOCKET_INIT,
	SOCKET_WRITE_STATE,
	SOCKET_RECEIVE_READ,
	SOCKET_SEND_READ,
	SOCKET_CLOSED
};

class Modem : public Serial {
public:
	Modem();
	~Modem();

	//Stuff subclasses have to implement
	virtual bool isRegistered() = 0;

	//Serial
	ModemResult parseATCommandResult(std::string strATCommand, std::string & strOutput, std::vector<std::string >& resultArray);
	bool sendATCommand(std::string strATCommand, unsigned int waitTIme = 250);
	bool sendATCommand(std::string strATCommand, std::string & strOutput, unsigned int waitTIme = 250);
	ModemResult sendAndParseATCommand(std::string strATCommand, std::vector<std::string >& resultArray, unsigned int waitTIme = 250);
	virtual void initModemSerialMode() = 0;

	//Hologram
	virtual std::string sendMessage(std::string message);

	//Cellular
	bool connect() {
		return manager.getConnection()->connect();
	}
	void disconnect() {
		manager.getConnection()->disconnect();
	}

	bool setTimezoneConfiguration();
	virtual void setNetworkRegistrationStatus() = 0;
	std::string popRecievedMessage();
	bool isPDPContextActive();
	bool setupPDPContext();
	void rebootModem();
	bool isConnected() {
		return isRegistered();
	}

	//Socket
	bool openReceiveSocket(int recv_port);
	virtual bool createSocket();
	bool connectSocket(std::string host, int port);
	bool listenSocket(int port);
	bool writeSocket(std::string data);
	std::string readSocket(int socketID, int bufferLen);
	bool closeSocket(int socketID);

	//URC
	void checkURC();
	void handleURC(std::string urcString);
	virtual void handleURCSMS(std::string urcString) = 0;
	virtual void handleURCLocation(std::string urcString) = 0;
	virtual void handleURCListen(std::string urcString) = 0;

	//SMS
	SMS popRecievedSMS();
	bool setSMSConfiguration();

	//MISC
	void setHexMode(bool state);
	std::string getName() {
		return name;
	}
	virtual void populateModemInformation() = 0;

	MODEM_INFO modemInfo;

	void setConnectionType(ConnectionType type, std::string name, std::string device) {
		manager.setConnectionType(type, name, device, this);
	}
	
#ifdef USERAS
	static std::vector<LPRASDEVINFO> getConnectedModems();
	static void getConnectionProfiles();
#endif
protected:
	bool checkRegistered(std::string atCommand);
	void parsePDU(std::string header, std::string pdu, SMS * sms, int & index);
	std::string parseSender(std::string pdu, int & offset);
	time_t parseTimestamp(std::string pdu, int & offset);
	std::string parseMessage(std::string pdu, int & offset);
	std::string name;
	URCState urcState;
	int last_read_payload_length;
	unsigned char socketId;
private:	
	std::deque<std::string> socketBuffer;
	ConnectionManager manager;
	ModemResult determineModemResult(std::string result);
};

