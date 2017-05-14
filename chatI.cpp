#include <chatI.h>

void
Chat::ChatServerI::LogOut(const ::Chat::UserPrx& callback,
                         const Ice::Current& current)
{
	for(int i =0; i < limboUserList.size(); i++){
		if(limboUserList[i]->getName().compare(callback->getName()) == 0){
			limboUserList.erase(limboUserList.begin() + i);
			return;
		}
	}
}

void
Chat::ChatServerI::LogIn(const ::Chat::UserPrx& callback,
                         const Ice::Current& current)
{
		for(int i=0; i < limboUserList.size(); i++){
			if(limboUserList[i]->getName().compare(callback->getName()) == 0){
				throw Chat::NameAlreadyExists();
				return;
			}
		}
    	limboUserList.push_back(callback);
    	std::cout << "User: " << callback->getName() << "joined server" << std::endl;
    	std::cout << "Users in group:" << std::endl;
    	for(int i=0;i<limboUserList.size();i++){
    		std::cout << limboUserList[i]->getName() << std::endl;
    	}
    	return;
}


::Chat::UserPrx
Chat::ChatServerI::getUserByName(const ::std::string& name,
                                 const Ice::Current& current)
{
    for(int i=0; i < limboUserList.size(); i++){
    	if(limboUserList[i]->getName().compare(name) == 0){
    		return limboUserList[i];
    	}
    }
    //throw Chat::NameDoesNotExist();
    return NULL;
}

::Chat::Groups
Chat::ChatServerI::GroupList(const Ice::Current& current)
{
    return groupList;
}

::Chat::GroupServerPrx
Chat::ChatServerI::getGroupServerByName(const ::std::string& name,
                                        const Ice::Current& current)
{
	for(int i=0; i<groupList.size(); i++){
		Chat::GroupServerPrx prx = groupList[i].get();
		if(prx->Name().compare(name) == 0){
			return prx;
		}
	}
	throw Chat::NameDoesNotExist();
}

void
Chat::ChatServerI::CreateGroup(const ::std::string& name,
                               const Ice::Current& current)
{
}

void
Chat::ChatServerI::DeleteGroup(const ::std::string& name,
                               const Ice::Current& current)
{

}

void
Chat::ChatServerI::registerServer(const ::Chat::GroupServerManagerPrx& serverManager,
                                  const Ice::Current& current)
{
	groupList.push_back(serverManager->ListGroups().back());
}

void
Chat::ChatServerI::unregisterServer(const ::Chat::GroupServerManagerPrx& serverManager,
                                    const Ice::Current& current)
{
}

::Chat::UserPrx
Chat::ChatServerI::getUser(const ::std::string& name,
                               const Ice::Current& current)
{
//	std::cout << "User: " << name << " created!" << std::endl;
//	UserPtr servant = new UserI(name);
//	return UserPrx::uncheckedCast(current.adapter->addWithUUID(servant));
	return NULL;
}

void
Chat::GroupServerI::join(const ::Chat::UserPrx& who,
                         const Ice::Current& current)
{
	for(int i =0; i<userList.size(); i++){
		if(userList[i]->getName().compare(who->getName()) == 0){
			throw Chat::UserAlreadyRegistered();
			return;
		}
	}
	userList.push_back(who);

}

void
Chat::GroupServerI::Leave(const ::Chat::UserPrx& who,
                          const Ice::Current& current)
{
	std::string name = who->getName();
	for(int i=0; i<userList.size(); i++){
		Chat::UserPrx prx = userList[i];
		if(prx->getName().compare(name) == 0){
			userList.erase(userList.begin()+i);
			return;
		}
	}
	throw Chat::UserDoesNotExist();
}

void
Chat::GroupServerI::SendMessage(const ::std::string& message,
                                const ::Chat::UserPrx& sender,
								const ::Chat::GroupServerPrx& gs,
                                const Ice::Current& current)
{
	bool isOnList = false;
	std::string nameSender = sender->getName();
	for(int i =0; i<userList.size(); i++){
		if(userList[i]->getName().compare(nameSender) == 0){
			isOnList = true;
		}
	}
	if(isOnList == true){
		for(int i=0; i<userList.size(); i++){
					if(userList[i]->getName().compare(nameSender) != 0){
						userList[i]->receiveText(message, sender, gs->Name());
					}
				}
	}else{
		throw Chat::UserDoesNotExist();
		return;
	}
}

::Chat::Users
Chat::GroupServerI::UserList(const Ice::Current& current)
{
    return userList;
}

::std::string
Chat::GroupServerI::Name(const Ice::Current& current)
{
    return name;
}

Chat::GroupServerI::GroupServerI(const ::std::string& pName){
	name = pName;
}

::Chat::GroupServerPrx
Chat::GroupServerManagerI::CreateGroup(const ::std::string& name,
                                       const Ice::Current& current)
{
	for(int i=0; i < groupList.size(); i++){
		if(groupList[i]->Name().compare(name) == 0){
			throw Chat::NameAlreadyExists();
			return NULL;
		}
	}
    GroupServerPtr servant = new GroupServerI(name);
    GroupServerPrx prx  = GroupServerPrx::uncheckedCast(current.adapter->addWithUUID(servant));
    groupList.push_back(prx);
    return prx;
}

::Chat::Groups
Chat::GroupServerManagerI::ListGroups(const Ice::Current& current)
{
    return groupList;
}

void
Chat::GroupServerManagerI::DeleteGroup(const ::std::string& name,
                                       const Ice::Current& current)
{
	for(int i=0; i<groupList.size(); i++){
		Chat::GroupServerPrx prx = groupList[i];
		if(prx->Name().compare(name) == 0){
			groupList.erase(groupList.begin()+i);
			return;
		}
	}
	throw Chat::NameDoesNotExist();
}

::Chat::GroupServerPrx
Chat::GroupServerManagerI::getGroupServerByName(const ::std::string& name,
                                                const Ice::Current& current)
{
	for(int i=0; i<groupList.size(); i++){
		Chat::GroupServerPrx prx = groupList[i];
		if(prx->Name().compare(name) == 0){
			return groupList[i];
		}
	}
	return NULL;
}

Chat::GroupServerManagerI::GroupServerManagerI(){
	Groups groupList;
}

Chat::UserI::UserI(const ::std::string& name){
	myName = name;
}

::std::string
Chat::UserI::getName(const Ice::Current& current)
{
    return myName;
}

void
Chat::UserI::receiveText(const ::std::string& msg,
                         const ::Chat::UserPrx& sender,
                         const ::std::string& serverName,
                         const Ice::Current& current)
{
	//this->myMessage = (sender->getName() + "@" + gServer->Name() + " : " + msg );
	std::cout << sender->getName() + "@" + serverName + " : " + msg << std::endl;
}


void
Chat::UserI::receivePrivateText(const ::std::string& msg,
                                const ::Chat::UserPrx& sender,
                                const Ice::Current& current)
{
	//this->myMessage = ("priv from " + sender->getName() + " : " + msg );
	std::cout << "priv from " + sender->getName() + " : " + msg << std::endl;
}


