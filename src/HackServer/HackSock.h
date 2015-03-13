#ifndef CHACKSOCK_H
#define	CHACKSOCK_H
#include "MyProtocol.h"
// CHackSock command target
/*
This is Server Socket which listens at port 258
for the connection.
*/
class CHackSock : public CSocket
{
private:
	//CArray<CRemoteSock*,CRemoteSock*> Con;//Let the server know how may connections are made;
public:
	CHackSock();
	virtual ~CHackSock();
	virtual void OnAccept(int nErrorCode);
};

#endif
