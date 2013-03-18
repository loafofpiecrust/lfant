#include <iostream>
#include <Network.hpp>

using namespace std;
using namespace sfs;

int main()
{
	Network* network = new Network();
	network->ConnectFCM(network->chatPeer, "127.0.0.1");
    cout << "Hello world!" << endl;
    return 0;
}
