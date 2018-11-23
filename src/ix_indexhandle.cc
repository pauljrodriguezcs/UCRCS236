//
// File:        ix_indexhandle.cc
// Description: IX_IndexHandle handles manipulations within the index
// Author:      <Your Name Here>
//

#include <unistd.h>
#include <sys/types.h>
#include "ix.h"
#include "pf.h"
#include "comparators.h"
#include <cstdio>

IX_IndexHandle::IX_IndexHandle() // Done
{
  // Implement this
  headerModified = false;
  openedIH = false;
}


IX_IndexHandle::~IX_IndexHandle() // Done
{
  // Implement this
  openedIH = false;
}

RC IX_IndexHandle::InsertEntry(void *pData, const RID &rid) // Needs to be finished
{
  // Implement this
  RC rc = 0;

  if(!openedIH){
    return IX_INVALIDINDEXFILE;
  }

  if(pData == NULL){
    return IX_INVALIDENTRY;
  }

  PF_PageHandle ph;
  PageNum page;

  if(header.rootPageNumber < 0){  // If no root exists, create it
    pfh.AllocatePage(ph);
    char *newData;

    if((rc = ph.GetPageNum(page))){
      return (rc);
    }

    if((rc = ph.GetData(newData))){
      return (rc);
    }

    if((rc = pfh.MarkDirty(page))){
      return (rc);
    }

    header.rootPageNumber = page;
    headerModified = true;

    /*

      Create leaf node

    */
  }

  else{
    pfh.GetThisPage(header.rootPageNumber, ph);
  }

  char *newData;

  if((rc = ph.GetData(newData))){
    return (rc);
  }

  /*

    Insert into tree (use newPage to verify it insert successfully)

  */

  if((rc = pfh.UnpinPage(header.rootPageNumber))){
    return (rc);
  }

  return (rc);
}

RC IX_IndexHandle::DeleteEntry(void *pData, const RID &rid) // Needs to be finished
{
  // Implement this
  if (!pData){
    return IX_INVALIDENTRY;
  }

  if(!openedIH){
    return IX_INVALIDINDEXFILE;
  }

  RC rc = 0;

  if(header.rootPageNumber < 0){
    return IX_INVALIDENTRY;
  }

  PF_PageHandle ph;

  if((rc = pfh.GetThisPage(header.rootPageNumber,ph))){
    return (rc);
  }

  /* 
  
    Delete entry from tree

  */

  if((rc = pfh.UnpinPage(header.rootPageNumber))){
    return (rc);
  }
  lastDeleted = rid;
  return (rc);
}

RC IX_IndexHandle::ForcePages() // Done
{
  // Implement this
  if(!openedIH){
    return IX_INVALIDINDEXFILE;
  }

  pfh.ForcePages(ALL_PAGES);
  return (0);
}
