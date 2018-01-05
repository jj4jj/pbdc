//This file is generated auto by pbdc library , dont edit it directly .
//Any problem when using it , please contact mail:hex@gamesci.com.cn 

#include "res.store.h"
#include "restb_loader.h"
#include <string>
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


int ResTabStorageAutoGen::Load(const std::string & dir){
    int iRetCode = 0;
    std::string strPath;
{%- for df in defs %}
{% if df.type == 'table' %}
    strPath = dir+"/TB{{df.name}}.pbin";
    iRetCode = _LoadTableFromBin(m_tb{{df.name}},strPath);
    CHECK_ERR_RET(iRetCode, "Load Bin Path:%s ERROR !", strPath.c_str());
{%- endif %}
{%- endfor %}   

    return 0;
}

