//
// File:        ix_indexscan.cc
// Description: IX_IndexHandle handles scanning through the index for a 
//              certain value.
// Author:      <Your name here>
//

#include <unistd.h>
#include <sys/types.h>
#include "pf.h"
#include "ix.h"
#include <cstdio>

IX_IndexScan::IX_IndexScan()    // Done
{
  // Implement this
  isOpen = false;
  leafIndex = 0;
}

IX_IndexScan::~IX_IndexScan()   // Done
{
  // Implement this
  if(isOpen){
    delete[] queryValue;
  }
}

RC IX_IndexScan::OpenScan(const IX_IndexHandle &indexHandle,
                CompOp compOp,
                void *value,
                ClientHint  pinHint)
{
  // Implement this
}

RC IX_IndexScan::GetNextEntry(RID &rid)
{
  /*
  RC rc = 0;
  // Implement this
  if(isOpen){
    return IX_BADINDEXSPEC;
  }

  if(!(ix_handle->isOpen)){
    return IX_BADINDEXSPEC;
  }

  if(!isFound){
    return IX_EOF;
  }

  PF_PageHandle ph;

  if(isOnOverflow){
    if(lastEmitted == ix_handle->lastDeleted && overflowIndex > 0){
      --overflowIndex;
    }

    if((rc = pf_handle->GetThisPage(currentOverflow,ph))){
      int unpin = currentOverflow;
      char *data;

      if((rc = ph.GetData(data))){
        return (rc));
      }


    }

  }
  */

}

RC IX_IndexScan::CloseScan()  // Done
{
  // Implement this
  if(!isOpen){
    return IX_BADINDEXSPEC;
  }

  isOpen = false;

  delete[] queryValue;

  return OK_RC;
}
