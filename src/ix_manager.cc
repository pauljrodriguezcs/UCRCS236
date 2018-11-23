//
// File:        ix_indexmanager.cc
// Description: IX_IndexHandle handles indexes
// Author:      Yifei Huang (yifei@stanford.edu)
//

#include <unistd.h>
#include <sys/types.h>
#include "ix.h"
#include "pf.h"
#include <climits>
#include <string>
#include <sstream>
#include <cstdio>
#include "comparators.h"


IX_Manager::IX_Manager(PF_Manager &pfm)     // Done
{
    pfManager = &pfm;
}

IX_Manager::~IX_Manager()   // Done
{

}

/*
 * Creates a new index given the filename, the index number, attribute type and length.
 */
RC IX_Manager::CreateIndex(const char *fileName, int indexNo,
                   AttrType attrType, int attrLength)       // Needs to be finished
{
    RC rc = 0;
    // Checks for correct attributes
    if (fileName == NULL){ 
        return IX_BADFILENAME; 
    }

    if (sizeof(fileName) > MAXNAME){ 
        return IX_BADINDEXSPEC; 
    }

    if (indexNo < 0){
        return IX_BADINDEXSPEC; 
    }

    if (attrType == INT && !(attrLength == 4)){ 
        return IX_BADINDEXSPEC; 
    }

    if (attrType == FLOAT && !(attrLength == 4)){ 
        return IX_BADINDEXSPEC; 
    }

    if (attrType == STRING && !((attrLength > 0) && attrLength < MAXSTRINGLEN)){ 
        return IX_BADINDEXSPEC; 
    }

    if (attrType == MBR && !(attrLength == sizeof(mbr_data))){ 
        return IX_BADINDEXSPEC; 
    }

    // create a file
    if((rc = pfManager->CreateFile(fileName))){
        return (rc);
    }
    
    // open the created file
    PF_PageHandle ph;
    PF_FileHandle fh;
    PageNum page;
    if((rc = pfManager->OpenFile(fileName,fh))){
        return (rc);
    }
    
    // allocate a new file header page
    if((rc = fh.AllocatePage(ph))){
        return (rc);
    }

    if((rc = ph.GetPageNum(page))){
        return (rc);
    }

    // mark the file header page dirty
    if((rc = fh.MarkDirty(page))){
        return (rc);
    }

    // fetch the contents of the page and update the header
    char *pData;

    if((rc = ph.GetData(pData))){
        return (rc);
    }
    
    IX_FileHeader header;

    header.attributeLength  = attrLength;
    header.rootPageNumber   = -1;

    /* finish this
    header->leafCapacity     = 
    header->internalCapacity =
    header->overflowCapacity =
    */

    header.headerPageNum    = page;
    header.attrType         = attrType;

    memcpy(pData,&header,sizeof(IX_FileHeader));

    // unpin the page
    if(rc = fh.UnpinPage(page)){
        return (rc);
    }

    // close the file
    if(rc = pfManager->CloseFile(fh)){
        return (rc);
    }
}

/*
 * This function destroys a valid index given the file name and index number.
 */
RC IX_Manager::DestroyIndex(const char *fileName, int indexNo)  // Done
{
    RC rc = 0;
    if(fileName == NULL){
        return (IX_BADFILENAME);
    }

    if((rc = pfManager->DestroyFile(fileName))){
        return (rc);
    }

    return OK_RC;
}

/*
 * This function, given a valid fileName and index Number, opens up the
 * index associated with it, and returns it via the indexHandle variable
 */
RC IX_Manager::OpenIndex(const char *fileName, int indexNo,
                 IX_IndexHandle &indexHandle)       // Done
{
    RC rc = 0;
    // Check if the index handle is already opened
    if(indexHandle.openedIH){
        return 1;
    }

    if(fileName == NULL){
        return IX_BADFILENAME;
    }

    if ((rc = pfManager->OpenFile(fileName,indexHandle.pfh))){
        return (rc);
    }

    PF_PageHandle header;

    if((rc = indexHandle.pfh.GetFirstPage(header))){
        return (rc);
    }

    char *data;

    if((rc = header.GetData(data))){
        return (rc);
    }

    memcpy(&indexHandle.header,data,sizeof(IX_FileHeader));

    PageNum page;

    if((rc = header.GetPageNum(page))){
        return (rc);
    }

    if ((rc = indexHandle.pfh.UnpinPage(page))){
        return (rc);
    }

    indexHandle.openedIH = true;
    indexHandle.headerModified = false;

    return (0);
}

/*
 * Given a valid index handle, closes the file associated with it
 */
RC IX_Manager::CloseIndex(IX_IndexHandle &indexHandle)  // Done
{
    RC rc = 0;
    // Makes sure index isn't closed already
    if(indexHandle.openedIH == false){
        return 1;
    }

    // fetch the header and update it if it has been modified
    if(indexHandle.headerModified){
        PF_PageHandle header;
        int headerPageNum = indexHandle.header.headerPageNum;

        if((rc = indexHandle.pfh.GetThisPage(headerPageNum,header))){
            return (rc);
        }

        if((rc = indexHandle.pfh.MarkDirty(headerPageNum))){
            return (rc);
        }

        char *pageContents;

        if((rc = header.GetData(pageContents))){
            return (rc);
        }

        memcpy(pageContents,&indexHandle.header,sizeof(IX_FileHeader));

        if((rc = indexHandle.pfh.UnpinPage(headerPageNum))){
            return (rc);
        }
    }

    if((rc = indexHandle.ForcePages())){
        return (rc);
    }

    // close the file
    if ((rc = pfManager->CloseFile(indexHandle.pfh))){
        return (rc);
    }

    // set the opened flags to false
    indexHandle.openedIH = false;
    indexHandle.headerModified = false;

    return (0);
}
