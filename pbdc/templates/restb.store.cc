{{declare}}
#include <string>
#include "{{file}}.store.h"
#include "gslog.h"


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

void ResTabStorageBase::OnClean() {
}

int ResTabStorageBase::OnLoad(){
    return 0;
}
void ResTabStorageBase::Clean() {
    OnClean();
}
int ResTabStorageBase::Load(){
    int iRetCode = 0;
    std::string strPath;
{%- for df in defs %}
{% if df.type == 'table' %}
    strPath = m_strLoadDir+"/TB{{df.name}}Desc.pbin";
    iRetCode = _LoadTableFromBin(m_tb{{df.name}},strPath);
    CHECK_ERR_RET(iRetCode, "Load Bin Path:%s ERROR !", strPath.c_str());
{%- endif %}
{%- endfor %}
    
    iRetCode = OnLoad();
    CHECK_ERR_RET(iRetCode, "custom load pbin error !");
    return iRetCode;
}

