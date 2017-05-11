
#include <Ice/Ice.h>
#include <chatI.h>
using namespace std;
using namespace Chat;

void showMenu(Groups gr){
	cout << "Press 1 to send message" << endl;
	cout << "Press 2 to join group server" << endl;
	cout << "Press 3 to create group server" << endl;

	if(gr.size() != 0){
		cout << "You are in groups:" << endl;
		for(int i=0; i< gr.size(); i++){
			cout << gr[i]->Name() << endl;
		}
	}
}

int main(int argc, char* argv[])
{
	int status = 0;
	Ice::CommunicatorPtr ic2;
	Ice::CommunicatorPtr ic;
	try {
		ic = Ice::initialize(argc, argv);
		ic2 = Ice::initialize(argc, argv);
		Ice::ObjectAdapterPtr adapter
					= ic->createObjectAdapterWithEndpoints("Adapter2", "default");
		string name;
		cin >> name;
		Ice::ObjectPtr objectUser = new UserI(name);
		Ice::ObjectPrx uPrxBase = adapter->addWithUUID(objectUser);
		adapter->activate();
		UserPrx uPrx = UserPrx::checkedCast(uPrxBase);
		Ice::ObjectPrx serverBase = ic->stringToProxy("ChatServer:default -p 10000");
		Ice::ObjectPrx groupManagerBase = ic->stringToProxy("Manager:default -p 10000");
		ChatServerPrx chatServer = ChatServerPrx::checkedCast(serverBase);
		if (!chatServer)
			throw "Invalid proxy chat";
		GroupServerManagerPrx groupManager = GroupServerManagerPrx::checkedCast(groupManagerBase);
		if(!groupManager)
			throw "Invalid proxy group";
		chatServer->LogIn(uPrx);
		Groups gr;
		while(true){
			showMenu(gr);
					int choice = 0;;
					cin >> choice;
					switch(choice){
					case 1: {
						char message[255];
						string name;
						cout << "Type username you want to send message" << endl;
						cin >> name;
						UserPrx proxy = chatServer->getUserByName(name);
						if(proxy != NULL){
							cout << "Type message: ";
							cin.ignore(1000, '\n');
							cin.getline(message,255);
							proxy->receivePrivateText(message, uPrx);
							break;
						}else{
							cout << "Proxy not found" << endl;
							break;
						}


					}
					case 2:{
						string name;
						cout << "Type chat you want to join to" << endl;
						cin >> name;
						GroupServerPrx proxy = chatServer->getGroupServerByName(name);
						if(proxy != NULL){
							cout << "Joined server" << endl;
							gr.push_back(proxy);
							break;
						}else{
							cout << "Group not found!" << endl;
							break;
						}
					}
					case 3:{
						string name;
						cout << "Type chat you want to create to" << endl;
						cin >> name;
						GroupServerPrx proxy = groupManager->CreateGroup(name);
						if(proxy != NULL){
							cout << "Created server" << endl;
							chatServer->registerServer(groupManager);
							break;
						}
					}
					default: {
						cout << "Wrong option!" << endl;
						break;
					}
					}
		}
	} catch (const Ice::Exception& ex) {
		cerr << ex << endl;
		status = 1;
	} catch (const char* msg) {
		cerr << msg << endl;
		status = 1;
	}
	if (ic)
		ic->destroy();
	if(ic2)
		ic2->destroy();
	return status;
}
