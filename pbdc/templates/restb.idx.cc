//This file is generated auto by pbdc library , dont edit it directly .
//Any problem when using it , please contact mail:hex@gamesci.com.cn 

//basic
#include "{{file}}.pb.h"
#include "{{file}}.idx.h"

//dcpots util
#include "error_code.h"


int   ResTabIndexBase::Build(){
    int iRetCode = 0;
{%-for df in defs %}
{%- if df.type == 'table' %}
    ////////////////build index of {{df.name}} begin ///////////
    if (m_pStorage->m_tb{{df.name}}.list_size() > {{df.options.limit_size}}){
        GLOG_ERR("Res Table:{{df.name}} reached max limit size > %d !", {{df.options.limit_size}});
        return -1;
    }
{%-if df.options.idx == 'list' %}
{%-else%}
    for (int i = 0 ;i < m_pStorage->m_tb{{df.name}}.list_size(); ++i){
    {%-if df.options.idx == 'id' %}
        int idx = m_pStorage->m_tb{{df.name}}.list(i).{{df.pkeys[0].n}};
        if(idx > {{df.id_max}} || idx <= 0){
            GLOG_ERR("Build id {{df.name}} index Error for 'ID:%d > MAX ID:%d or Invalid Range' ", idx, {{df.id_max}});
            return -1;
        }
        for(int j = (int)m_pStorage->m_idx{{df.name}}.size();  j < idx+1 ;++j){
            m_pStorage->m_idx{{df.name}}.push_back(nullptr);
        }
        if (m_pStorage->m_idx{{df.name}}[idx]){
            GLOG_ERR("Build id {{df.name}} index Error for 'id:%d is repeatly ' Info:%s", idx,m_pStorage->m_tb{{df.name}}.list(i).ShortDebugString().c_str());
            return -1;
        }
        m_pStorage->m_idx{{df.name}}[idx] = &(m_pStorage->m_tb{{df.name}}.list(i));
    {%-elif df.options.idx == 'hash' or df.options.idx == 'tree' %}
        {{df.name}}Key  key{{df.name}};
        {%-for fd in df.pkeys %}
        key{{df.name}}.{{fd.n}} = m_pStorage->m_tb{{df.name}}.list(i).{{fd.n}};
        {%-endfor%}
        auto pair = m_pStorage->m_idx{{df.name}}.insert(key{{df.name}}, &(m_pStorage->m_tb{{df.name}}.list(i)));
        if (pair.second){
            GLOG_ER("Build Hash/Tree index {{df.name}} error for key is repeatly row:%d Info(%s)!", i, m_pStorage->m_tb{{df.name}}.list(i).ShortDebugString().c_str());
            return -1;
        }
    {%-else%}
    {%endif%}
    }
    ////////////////build index of {{df.name}} end ///////////
{%endif%}
{%- endif %}
{%- endfor %}
    //user build 
    iRetCode = OnBuild();
    CHECK_ERR_RET(iRetCode, "build custom index error !");
    return iRetCode;
}

int ResTabIndexBase::OnBuild (){
    return 0;    
}

void ResTabIndexBase::OnClean(){

}

void   ResTabIndexBase::Clean(){
    OnClean();
    ///////////////////////
{%-for df in defs %}
{%-if df.type == 'table' %}
{%-if df.options.idx == 'id' or df.options.idx == 'hash' or df.options.idx == 'tree' %}
    m_idx{{df.name}}.clear();
{%-endif%}
{%-endif %}
{%-endfor %}
}

{% for df in defs %}
{% if df.type == 'table' %}
//{{df.name}} accessor
const {{package}}::TB{{df.name}}Desc &  ResTabIndexBase::{{df.name}}DescGetList() const {
    return m_pStorage->m_tb{{df.name}};
}
{%-if df.options.idx == 'id' %}
const {{package}}::{{df.name}}Desc *  ResTabIndexBase::{{df.name}}DescGetBy{{df.pkeys[0].n|camel_style_name}}(int32_t {{df.pkeys[0].n}}) const {
    if({{df.pkeys[0].n}} >= m_idx{{df.name}}.size()){
        return nullptr;
    }
    return m_idx{{df.name}}[{{df.pkeys[0].n}}];
}
{%-elif df.options.idx == 'list' %}
const {{package}}::{{df.name}}Desc *  ResTabIndexBase::{{df.name}}DescGetByKey(const {{df.name}}Key & key) const {
    for(int i = 0 ;i < m_pStorage->m_tb{{df.name}}.list_size(); ++i){
        if (
        {%-for fd in df.pkeys %}
            m_pStorage->m_tb{{df.name}}.list(i).{{fd.n}} == key.{{fd.n}}{%if loop.last %} ) {%else%} && {%endif%}
        {%-endfor %} {
            return  &(m_pStorage->m_tb{{df.name}}.list(i));
        }
    }
    return nullptr;
}
{%-elif df.options.idx == 'hash' %}
const {{package}}::{{df.name}}Desc *  ResTabIndexBase::{{df.name}}DescGetByKey(const {{df.name}}Key & key) const {
    {{df.name}}HashMapItr it = m_idx{{df.name}}.find(key);
    if (it == m_idx{{df.name}}.end()){
        return nullptr;
    }
    return it->second;
}
{%elif df.options.idx == 'tree' %}
const {{package}}::{{df.name}}Desc *  ResTabIndexBase::{{df.name}}DescGetByKey(const {{df.name}}Key & key) const {
    {{df.name}}TreeMapItr it = m_idx{{df.name}}.find(key);
    if (it == m_idx{{df.name}}.end()){
        return nullptr;
    }
    return it->second;
}
const {{package}}::{{df.name}}Desc *  ResTabIndexBase::{{df.name}}DescUpperBoundByKey(const {{df.name}}Key & key) const {
    {{df.name}}TreeMapItr it = m_idx{{df.name}}.upper_bound(key);
    if (it == m_idx{{df.name}}.end()){
        return nullptr;
    }
    return it->second;
}
const {{package}}::{{df.name}}Desc *  ResTabIndexBase::{{df.name}}DescLowerBoundByKey(const {{df.name}}Key & key) const {
    {{df.name}}TreeMapItr it = m_idx{{df.name}}.lower_bound(key);
    if (it == m_idx{{df.name}}.end()){
        return nullptr;
    }
    return it->second;
}
const std::vector<const {{package}}::{{df.name}}Desc *> &       ResTabIndexBase::{{df.name}}DescRangeByKey(std::vector<{{package}}::{{df.name}}Desc *> vResult , const {{df.name}}Key & key) const {
    vResult.clear();
    auto std::pair<{{df.name}}TreeMapItr,{{df.name}}TreeMapItr> range = m_idx{{df.name}}.equal_range(key);
    for(auto it = range.first; it != range.second; ++it){
        if(it->second){
            vResult.push_back(it->second);
        }
    }
    return vResult;
}
{%endif%}
{%- endif %}
{%- endfor %}