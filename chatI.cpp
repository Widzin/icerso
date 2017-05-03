#include <chatI.h>

void
Chat::ChatServerI::LogIn(const ::Chat::UserPrx& callback,
                         const Ice::Current& current)
{
}

::Chat::UserPrx
Chat::ChatServerI::getUserByName(const ::std::string& name,
                                 const Ice::Current& current)
{
    return 0;
}

::Chat::Groups
Chat::ChatServerI::GroupList(const Ice::Current& current)
{
    return ::Chat::Groups();
}

::Chat::GroupServerPrx
Chat::ChatServerI::getGroupServerByName(const ::std::string& name,
                                        const Ice::Current& current)
{
    return 0;
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
	std::cout << "User: " << name << " created!" << std::endl;
	UserPtr servant = new UserI(name);
	return UserPrx::uncheckedCast(current.adapter->addWithUUID(servant));
}

void
Chat::GroupServerI::join(const ::Chat::UserPrx& who,
                         const Ice::Current& current)
{
	userList.push_back(who);
	std::cout << "User: " << who->getName() << "joined server" << std::endl;
	std::cout << "Users in group:" << std::endl;
	for(int i=0;i<userList.size();i++){
		std::cout << userList[i]->getName() << std::endl;
	}

}

void
Chat::GroupServerI::Leave(const ::Chat::UserPrx& who,
                          const Ice::Current& current)
{
}

void
Chat::GroupServerI::SendMessage(const ::std::string& message,
                                const ::Chat::UserPrx& sender,
                                const Ice::Current& current)
{
}

::Chat::Users
Chat::GroupServerI::UserList(const Ice::Current& current)
{
    return ::Chat::Users();
}

::std::string
Chat::GroupServerI::Name(const Ice::Current& current)
{
    return ::std::string();
}

::Chat::GroupServerPrx
Chat::GroupServerManagerI::CreateGroup(const ::std::string& name,
                                       const Ice::Current& current)
{
    return 0;
}

::Chat::Groups
Chat::GroupServerManagerI::ListGroups(const Ice::Current& current)
{
    return ::Chat::Groups();
}

void
Chat::GroupServerManagerI::DeleteGroup(const ::std::string& name,
                                       const Ice::Current& current)
{
}

::Chat::GroupServerPrx
Chat::GroupServerManagerI::getGroupServerByName(const ::std::string& name,
                                                const Ice::Current& current)
{
    return 0;
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
                         const ::Chat::GroupServerPrx& gServer,
                         const Ice::Current& current)
{
}

void
Chat::UserI::receivePrivateText(const ::std::string& msg,
                                const ::Chat::UserPrx& sender,
                                const Ice::Current& current)
{
}
