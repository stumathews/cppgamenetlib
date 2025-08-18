#include "TcpNetworkSocket.h"
#include "Networking.h"

bool gamenetlib::TcpNetworkSocket::IsTcp()
{
	return true;
}

int gamenetlib::TcpNetworkSocket::Send(const char* data, const int dataLength, const unsigned long deltaMs)
{
	return Networking::netSendVRec(socket, data, dataLength);
}

int gamenetlib::TcpNetworkSocket::Receive(char* readBuffer, const int bufLength, unsigned long deltaMs)
{
	return Networking::netReadVRec(socket, readBuffer, bufLength);
}

bool gamenetlib::TcpNetworkSocket::IsValid()
{
	return socket;
}

SOCKET gamenetlib::TcpNetworkSocket::GetRawSocket() const
{
	return socket;
}

void gamenetlib::TcpNetworkSocket::Connect(const char* address, const char* port)
{
	auto* networking = Networking::Get();
	socket = networking->netTcpClient(address, port);
}
