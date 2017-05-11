#ifndef CHAT_I_H
#define CHAT_I_H

#include <chat.h>
#include <Ice/ObjectAdapter.h>
namespace Chat
{

class ChatServerI : virtual public ChatServer
{

public:
	Users limboUserList;
	Groups groupList;
    virtual void LogIn(const ::Chat::UserPrx&,
                       const Ice::Current&);

    virtual ::Chat::UserPrx getUserByName(const ::std::string&,
                                          const Ice::Current&);

    virtual ::Chat::Groups GroupList(const Ice::Current&);

    virtual ::Chat::GroupServerPrx getGroupServerByName(const ::std::string&,
                                                        const Ice::Current&);

    virtual void CreateGroup(const ::std::string&,
                             const Ice::Current&);

    virtual void DeleteGroup(const ::std::string&,
                             const Ice::Current&);

    virtual void registerServer(const ::Chat::GroupServerManagerPrx&,
                                const Ice::Current&);

    virtual void unregisterServer(const ::Chat::GroupServerManagerPrx&,
                                  const Ice::Current&);

    virtual ::Chat::UserPrx getUser(const ::std::string& name,
                                   const Ice::Current& current);
};

class GroupServerI : virtual public GroupServer
{

public:
	Users userList;
	std::string name;

	GroupServerI(const std::string& pName);
    virtual void join(const ::Chat::UserPrx&,
                      const Ice::Current&);

    virtual void Leave(const ::Chat::UserPrx&,
                       const Ice::Current&);

    virtual void SendMessage(const ::std::string&,
                             const ::Chat::UserPrx&,
                             const Ice::Current&);

    virtual ::Chat::Users UserList(const Ice::Current&);

    virtual ::std::string Name(const Ice::Current&);
};

class GroupServerManagerI : virtual public GroupServerManager
{
public:
	Groups groupList;

	GroupServerManagerI();

    virtual ::Chat::GroupServerPrx CreateGroup(const ::std::string&,
                                               const Ice::Current&);

    virtual ::Chat::Groups ListGroups(const Ice::Current&);

    virtual void DeleteGroup(const ::std::string&,
                             const Ice::Current&);

    virtual ::Chat::GroupServerPrx getGroupServerByName(const ::std::string&,
                                                        const Ice::Current&);
};

class UserI : virtual public User
{
public:
	std::string myName;

	UserI(const ::std::string& name);

    virtual ::std::string getName(const Ice::Current&);

    virtual void receiveText(const ::std::string&,
                             const ::Chat::UserPrx&,
                             const ::std::string&,
                             const Ice::Current&);

    virtual void receivePrivateText(const ::std::string&,
                                    const ::Chat::UserPrx&,
                                    const Ice::Current&);
};

}

#endif
