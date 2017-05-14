
#include <Ice/Ice.h>
#include <chatI.h>
using namespace std;
using namespace Chat;

void showMenu(Groups gr){
	cout << "Press 1 to send private message" << endl;
	cout << "Press 2 to join group server" << endl;
	cout << "Press 3 to create group server" << endl;

	if(gr.size() != 0){
		cout << "Press 4 to leave group" << endl;
		cout << "Press 5 to send group message" << endl;
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
		cout << "Enter your name: ";
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
		try{
			chatServer->LogIn(uPrx);
		}catch(NameAlreadyExists& e){
			cout << "Name already exist" << endl;
			ic->destroy();
			ic2->destroy();
			return status = 1;
		}
		Groups gr;
		while(status == 0){
			showMenu(gr);
					int choice = 0;
					cin >> choice;
					switch(choice){
					case 1: {
						char message[255];
						string name;
						cout << "Type username you want to send message" << endl;
						cin >> name;
						UserPrx proxy = NULL;
						try{
							 proxy = chatServer->getUserByName(name);
						}catch(NameDoesNotExist& e){
							cout << "Name does not exist!";
							status = 1;
							break;
						}
						if(proxy == NULL){
							cout << "Name does not exist!";
							status = 1;
							break;
						}
						cout << "Type message: ";
						cin.ignore(1000, '\n');
						cin.getline(message,255);
						proxy->receivePrivateText(message, uPrx);
						break;
					}
					case 2:{
						string name;
						cout << "Type chat you want to join to" << endl;
						cin >> name;
						GroupServerPrx proxy = NULL;
						try{
							proxy = chatServer->getGroupServerByName(name);
						}catch(NameDoesNotExist& e){
							cout << "Name does not exist!" << endl;
							status =1;
							break;
						}
						cout << "Joined server" << endl;
						try{
							proxy->join(uPrx);
						}catch(UserAlreadyRegistered& e){
							cout << "User Already registered!" << endl;
							status =1;
							break;
						}
						gr.push_back(proxy);
						break;
					}
					case 3:{
						string name;
						cout << "Type chat you want to create" << endl;
						cin >> name;
						GroupServerPrx proxy;
						try{
							proxy = groupManager->CreateGroup(name);
						}catch(NameAlreadyExists& e){
							cout << " Name already exist!" << endl;
							status = 1;
							break;
						}
						cout << "Created server" << endl;
						chatServer->registerServer(groupManager);
						break;
					}
					case 4:{
						//leave group
						if(gr.size() > 0){
							string name;
							cout << "Type name of group you want to leave" << endl;
							cin >> name;
							for(int i =0; i < gr.size(); i++){
								if(gr[i]->Name().compare(name) == 0){
									try{
										gr[i]->Leave(uPrx);
									}catch(UserDoesNotExist& e){
										cout << "User does not exist" << endl;
										status = 1;
										break;
									}
									gr.erase(gr.begin() + i);
									break;
								}else{
									cout << "You are not in group " + name << endl;
									break;
								}
							}
							break;
						}else{

						break;
						}
					}

					case 5:{
						//send group
						if(gr.size() > 0){
							string name;
							cout << "Type group name you want to send message" << endl;
							cin >> name;
							for(int i =0; i < gr.size(); i++){
								if(gr[i]->Name().compare(name) == 0){
									char message[256];
									cout << "Type message: ";
									cin.ignore(1000, '\n');
									cin.getline(message,255);
									try{
										gr[i]->SendMessage(message, uPrx, gr[i]);
									}catch(UserDoesNotExist& e){
										cout << "User does not exist!" << endl;
										status =1 ;
										break;
									}
									break;
								}else{
									cout << "You are not in group " + name << endl;
									break;
								}
							}
							break;
						}else{
							break;
						}
					}
					default: {
						cout << "Wrong option!" << endl;
						break;
					}
					}
		}
		for(int i =0; i < gr.size(); i++){
			gr[i]->Leave(uPrx);
		}
		chatServer->LogOut(uPrx);
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
