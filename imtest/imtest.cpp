#include "main.h"
#include "User.h"
#include "Nick.h"
#include "Modules.h"
#include "Chan.h"

class CIMtestModule : public CModule {
public:
	MODCONSTRUCTOR(CIMtestModule) {}
	
	virtual bool OnLoad(const CString& sArgs, CString& sErrorMsg) {
		return true;
	}

	virtual void OnModCommand(const CString& sCommand) {
		if(sCommand.Token(0).Equals("URAW")) {
			PutUser(sCommand.Token(1, true));
		} else if(sCommand.Token(0).Equals("IRAW")) {
			PutIRC(sCommand.Token(1, true));
		}
	}
	
	virtual EModRet OnChanMsg(CNick& Nick, CChan& Channel, CString& sMessage) {
		PutModule("[" + Channel.GetName() + " :: " + Nick.GetNick() + "] " + sMessage);
		return CONTINUE;
	}

	virtual EModRet OnUserJoin(CString& sChannel, CString &sKey) {
		PutModule("Join Channel: " + sChannel);
		return CONTINUE;
	}

	virtual EModRet OnUserRaw(CString &sLine) {
		if(sLine.Token(0).Equals("ISON")) return CONTINUE; // hide the ISON query as it happens a lot

		PutModule("Raw: " + sLine);

		if(sLine.Token(0).Equals("JOIN") && sLine.Token(1).Equals("#imtest")) {
			PutUser(":znc.myserver.com 353 jsjohnst = #imtest :jsjohnst buddy0 buddy1 buddy2 buddy3 buddy4 buddy5 buddy6 buddy7");
			PutUser(":znc.myserver.com 366 jsjohnst #imtest :End of /NAMES list.");
			PutUser(":znc.myserver.com 324 jsjohnst #imtest +cnt");
			PutUser(":znc.myserver.com 329 jsjohnst #imtest 1290545724");
		}

		if(sLine.Token(0).Equals("WHO") && sLine.Token(1).Equals("#imtest")) {
			PutUser(":znc.myserver.com 352 jsjohnst #imtest ~buddy0 localhost znc.myserver.com buddy0 G@ :0 Away ChannelOp");
			PutUser(":znc.myserver.com 352 jsjohnst #imtest ~buddy1 localhost znc.myserver.com buddy1 G+ :0 Away Voice");
			PutUser(":znc.myserver.com 352 jsjohnst #imtest ~buddy2 localhost znc.myserver.com buddy2 G :0 Away");
			PutUser(":znc.myserver.com 352 jsjohnst #imtest ~buddy3 localhost znc.myserver.com buddy3 G* :0 Away IrcOp");
			PutUser(":znc.myserver.com 352 jsjohnst #imtest ~buddy4 localhost znc.myserver.com buddy4 H :0 Active");
			PutUser(":znc.myserver.com 352 jsjohnst #imtest ~buddy5 localhost znc.myserver.com buddy5 H+ :0 Active Voice");
			PutUser(":znc.myserver.com 352 jsjohnst #imtest ~buddy6 localhost znc.myserver.com buddy6 H* :0 Active IrcOp");
			PutUser(":znc.myserver.com 352 jsjohnst #imtest ~buddy7 localhost znc.myserver.com buddy7 H@ :0 Active ChannelOp");
			PutUser(":znc.myserver.com 315 jsjohnst #imtest :End of /WHO list.");
		}

		if(sLine.Token(1).Equals("#imtest")) {
			return HALT;
		}

		return CONTINUE;
	}

	virtual EModRet OnRaw(CString &sLine) {
		if(sLine.Token(1).Equals("303")) return CONTINUE; // hide the ISON reply as it happens a lot

		PutModule("IRC Raw: " + sLine);
		return CONTINUE;
	}
};

MODULEDEFS(CIMtestModule, "Module for sending testing a fake IM channel")
