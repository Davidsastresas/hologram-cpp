#include "Cloud\HologramCloud.h"
#include <iostream>

int main()
{
	std::cout << "Testing Hologram SDK" << std::endl;
	std::map<std::string, std::string> credentials;
	std::cout << "Building Credentials" << std::endl;
	credentials.emplace("devicekey", "lWu,B9Qm");
	HologramCloud cloud = HologramCloud(credentials, false, NetworkType::CELLULAR);
	std::vector<std::wstring> topics;
	std::cout << "Adding Topics" << std::endl;
	topics.push_back(L"WINDOWSIOTSDK");
	std::cout << "Sending Message" << std::endl;
	cloud.sendMessage(L"This is a test from windows iot", topics);
	return 0;
}

