#include "UdpConnectedNetworkSocket.h"
#include "Networking.h"

gamenetlib::UdpConnectedNetworkSocket::UdpConnectedNetworkSocket(): socket(0)
{
	
}

void gamenetlib::UdpConnectedNetworkSocket::Connect(const char* address, const char* port)
{
	auto* networking = Networking::Get();
	socket = networking->netConnectedUdpClient(address, port);
}

bool gamenetlib::UdpConnectedNetworkSocket::IsTcp()
{
	return false;
}

int gamenetlib::UdpConnectedNetworkSocket::Send(const char* data, const int dataLength, const unsigned long deltaMs)
{
	return send(socket, data, dataLength, 0);
}

int gamenetlib::UdpConnectedNetworkSocket::Receive(char* readBuffer, const int bufLength, unsigned long deltaMs)
{
	return recv(socket, readBuffer, bufLength, 0); 
}

bool gamenetlib::UdpConnectedNetworkSocket::IsValid()
{
	return socket;
}

SOCKET gamenetlib::UdpConnectedNetworkSocket::GetRawSocket() const
{
	return socket;
}
