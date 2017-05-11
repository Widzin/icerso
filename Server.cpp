
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
		Ice::ObjectAdapterPtr adapter
			= ic->createObjectAdapterWithEndpoints("Adapter", "default -p 10000");
		Ice::ObjectPtr objectChat = new ChatServerI;
		Ice::ObjectPtr objectManager = new GroupServerManagerI;
		//Ice::ObjectPtr objectManager = new

		adapter->add(objectChat, ic->stringToIdentity("ChatServer"));
		adapter->add(objectManager, ic->stringToIdentity("Manager"));
		adapter->activate();
		cout << "Server started" << endl;
		ic->waitForShutdown();
	} catch (const Ice::Exception& e) {
		cerr << e << endl;
		status = 1;
	} catch (const char* msg) {
		cerr << msg << endl;
		status = 1;
	}
	if (ic) {
		try {
			ic->destroy();
		} catch (const Ice::Exception& e) {
			cerr << e << endl;
			status = 1;
		}
	}
	return status;
}
