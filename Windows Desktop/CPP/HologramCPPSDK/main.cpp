#include "Cloud\HologramCloud.h"
#include <iostream>

int main()
{
	std::map<std::string, std::string> credentials;
	credentials.emplace("devicekey", "lWu,B9Qm");
	HologramCloud cloud = HologramCloud(credentials, false, NetworkType::CELLULAR);
	std::vector<std::wstring> topics;
	topics.push_back(L"WINDOWSSDK");
	cloud.sendMessage(L"This is a test", topics);
	return 0;
}