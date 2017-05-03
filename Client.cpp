
#include <Ice/Ice.h>
#include <chatI.h>
using namespace std;
using namespace Chat;

int main(int argc, char* argv[])
{
	int status = 0;
	Ice::CommunicatorPtr ic;
	try {
		ic = Ice::initialize(argc, argv);
		Ice::ObjectPrx serverBase = ic->stringToProxy("ChatServer:default -p 10000");
		Ice::ObjectPrx groupBase = ic->stringToProxy("GroupServer:default -p 10000");
		ChatServerPrx chatServer = ChatServerPrx::checkedCast(serverBase);
		if (!chatServer)
			throw "Invalid proxy chat";
		GroupServerPrx groupServer = GroupServerPrx::checkedCast(groupBase);
		if(!groupServer)
			throw "Invalid proxy group";
		string name;
		cin >> name;
		UserPrx user = chatServer->getUser(name);
		groupServer->join(user);
		while(true);
	} catch (const Ice::Exception& ex) {
		cerr << ex << endl;
		status = 1;
	} catch (const char* msg) {
		cerr << msg << endl;
		status = 1;
	}
	if (ic)
		ic->destroy();
	return status;
}
