{{declare}}
#include "{{file}}.pb.h"
#include "{{file}}.idx.h"
#include "{{file}}.verify.h"
#include "gslog.h"

int         ResTabVerifierBase::OnVerify(){
    return 0;
}
void         ResTabVerifierBase::OnClean() {
}
void         ResTabVerifierBase::Clean() {
    OnClean();
}
int         ResTabVerifierBase::Verify(){
    int iRetCode = 0;
    //check foreign_key constraints
{%-for df in defs %}
{%- if df.type == 'table' %}
    ////////////////check constraints of {{df.name}} begin ///////////
    for(int i = 0 ;i < m_pIndex->{{df.name}}DescGetList().list_size(); ++i){
        const {{package}}::{{df.name}}Desc & rDesc = m_pIndex->{{df.name}}DescGetList().list(i);
        //check fd ? todo
    }
    ////////////////check constraints of {{df.name}} end ///////////
{% endif %}
{%- endfor %}
    iRetCode = OnVerify();    
    CHECK_ERR_RET(iRetCode, "check custom constraints error !");
    return 0;
}

