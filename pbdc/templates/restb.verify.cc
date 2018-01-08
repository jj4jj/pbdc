//This file is generated auto by pbdc library , dont edit it directly .
//Any problem when using it , please contact mail:hex@gamesci.com.cn 

#ifndef _RES_TAB_{{package}}_VERIFY_AUTO_GEN_H_
#define _RES_TAB_{{package}}_VERIFY_AUTO_GEN_H_

#include "{{file}}.index.h"
//dcpots util
#include "dcpots/base/logger.h"

int         ResTabVerifierBase::OnVerify(){
    return 0;
}
int         ResTabVerifierBase::Verify(){
    int iRetCode = 0;
    //check foreign_key constraints
{%-for df in defs %}
{%- if df.type == 'table' %}
    ////////////////check constraints of {{df.name}} begin ///////////
    for(int i = 0 ;i < m_pIndex.{{df.name}}DescGetList->list_size(); ++i){
        const {{df.name}}Desc & rDesc = m_pIndex->{{df.name}}DescGetList.list(i);
        //check fd ? todo
    }
    ////////////////check constraints of {{df.name}} end ///////////
{% endif %}
{%- endfor %}
    iRetCode = OnVerify();    
    CHECK_ERR_RET(iRetCode, "check custom constraints error !");
    return 0;
}

#endif
