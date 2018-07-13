#pragma once
#include "SerialComm.h"
#include "MsgBuffer.h"

class CCliProtocol :
  public CSerialComm
{
public:
  CCliProtocol();
  CCliProtocol(CString strName, DWORD64 hAddress);
  ~CCliProtocol();

// attributes
public:
  CMsgBuffer	m_msgBuf;



// implementation
public:
	bool	ProcessRxByte( BYTE nChar );
	void	FormatTxBuffer( CMsgBuffer* pBuffer );

  virtual void  SendMessage(CMsgBuffer* pMsgBuffer, int iTimeout);

};

