//This file is generated auto by pbdc library , dont edit it directly .
//Any problem when using it , please contact mail:hex@gamesci.com.cn 

#include <string>
#include "{{file}}.store.h"

//dcpots util
#include "dcpots/base/logger.h"


template <typename MsgT>
static int _LoadTableFromBin(MsgT & msg, std::string & file ){
    std::string strBinBuff;
    int iRetCode = dcs::readfile(file, strBinBuff);
    if(iRetCode){
        GLOG_ERR("read file:%s error ret:%d", file.c_str(), iRetCode);
        return -1;
    }
    if (msg.ParseFromString(strBinBuff)){
        GLOG_ERR("parse from pbin error !");
        return -1;
    }
    return 0;
}

ResTabStorageBase::ResTabStorageBase(const std::string & dir):m_strLoadDir(dir){    
}

int ResTabStorageAutoGen::OnLoad(){
    return 0;
}

int ResTabStorageAutoGen::Load(){
    int iRetCode = 0;
    std::string strPath;
{%- for df in defs %}
{% if df.type == 'table' %}
    strPath = m_strDir+"/TB{{df.name}}.pbin";
    iRetCode = _LoadTableFromBin(m_tb{{df.name}},strPath);
    CHECK_ERR_RET(iRetCode, "Load Bin Path:%s ERROR !", strPath.c_str());
{%- endif %}
{%- endfor %}
    
    iRetCode = OnLoad();
    CHECK_ERR_RET(iRetCode, "custom load pbin error !");
    return iRetCode;
}

