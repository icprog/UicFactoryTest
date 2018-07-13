#include "CliProtocol.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCliProtocol::CCliProtocol()
{
}

CCliProtocol::~CCliProtocol()
{
}

// overrides
bool CCliProtocol::ProcessRxByte(BYTE nData)
{
  bool fMessageComplete = false;

  // check for terminating character
  if (nData == '\r')
  {
    // set complete flag
    fMessageComplete = true;
  }
  else
  {
    // stuff in buffer
    m_mbufLclMsg.AddToMsgRxBuffer(nData);
  }

  // return the complete status
  return(fMessageComplete);
}

void CCliProtocol::FormatTxBuffer(CMsgBuffer* pBuffer)
{
  // set the transmit raw to msg
  pBuffer->m_iTxRawLen = pBuffer->m_iTxMsgLen;

  // copy the msg to raw
  memcpy(pBuffer->m_anTxRawBuf, pBuffer->m_anTxMsgBuf, pBuffer->m_iTxMsgLen);

  // add the terminator
  pBuffer->AddToRawTxBuffer('\r');
}

void CCliProtocol::SendMessage(CMsgBuffer* pMsgBuffer, int iTimeout)
{
  // copy the message buffer to the local
  m_msgBuf.CopyMessage(pMsgBuffer);

  // send it
  SendXmtBuffer(&m_msgBuf, iTimeout );
}
