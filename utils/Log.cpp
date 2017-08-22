#include "Log.h"
#include "Time.h"

/*
Log::Log(Params *p) {
    par = p;
    firstTime = false;
}


Log::Log(const Log &anotherLog) {
    this->par = anotherLog.par;
    this->firstTime = anotherLog.firstTime;
}


Log& Log::operator = (const Log& anotherLog) {
    this->par = anotherLog.par;
    this->firstTime = anotherLog.firstTime;
    return *this;
}*/

Log::~Log() {}

/**
 * FUNCTION NAME: LOG
 *
 * DESCRIPTION: Print out to file dbg.log, along with Address of node.
 */
void Log::LOG(const network::Address& addr, const char * str, ...) {

    /*static FILE *fp;
    static FILE *fp2;
    va_list vararglist;
    static char buffer[30000];
    static int numwrites;
    static char stdstring[30];
    static char stdstring2[40];
    static char stdstring3[40]; 
    static int dbg_opened=0;

    // std::lock_guard<std::mutex> lock_guard(mutex);

    if(dbg_opened != 639){
        numwrites=0;

        stdstring2[0]=0;

        strcpy(stdstring3, stdstring2);

        strcat(stdstring2, DBG_LOG);
        strcat(stdstring3, STATS_LOG);

        fp = fopen(stdstring2, "w");
        fp2 = fopen(stdstring3, "w");

        dbg_opened=639;
    }
    else 

    sprintf(stdstring, "%s ", addr.toString());

    va_start(vararglist, str);
    vsprintf(buffer, str, vararglist);
    va_end(vararglist);

    if (!firstTime) {
        int magicNumber = 0;
        std::string magic = MAGIC_NUMBER;
        int len = magic.length();
        for ( int i = 0; i < len; i++ ) {
            magicNumber += (int)magic.at(i);
        }
        fprintf(fp, "%x\n", magicNumber);
        firstTime = true;
    }

    if(memcmp(buffer, "#STATSLOG#", 10)==0){
        fprintf(fp2, "\n %s", stdstring);
        fprintf(fp2, "[%d] ", par->getcurrtime());

        fprintf(fp2, buffer);
    }
    else{
        fprintf(fp, "\n %s", stdstring);
        fprintf(fp, "[%d] ", par->getcurrtime());
        fprintf(fp, buffer);

    }

    if(++numwrites >= MAXWRITES){
        fflush(fp);
        fflush(fp2);
        numwrites=0;
    }*/
}

/**
 * FUNCTION NAME: logNodeAdd
 *
 * DESCRIPTION: To Log a node add
 */
void Log::logNodeAdd(const network::Address& thisNode, const network::Address& addedAddr) {
    static char stdstring[100];
    sprintf(stdstring, "Node %s joined at time %d", thisNode.toString(), utils::Time::getCurrentTime();
    LOG(thisNode, stdstring);
}

/**
 * FUNCTION NAME: logNodeRemove
 *
 * DESCRIPTION: To log a node remove
 */
void Log::logNodeRemove(const network::Address& thisNode, const network::Address& removedAddr) {
    static char stdstring[30];
    sprintf(stdstring, "Node %s removed at time %d", thisNode.toString(), utils::Time::getCurrentTime());
    LOG(thisNode, stdstring);
}

