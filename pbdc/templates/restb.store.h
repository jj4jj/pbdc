//This file is generated auto by pbdc library , dont edit it directly .
//Any problem when using it , please contact mail:hex@gamesci.com.cn 

#ifndef _RES_TAB_{{package}}_STORAGE_AUTO_GEN_H_
#define _RES_TAB_{{package}}_STORAGE_AUTO_GEN_H_

{%for pkg in imports %}
#include "{{pkg}}.pb.h";
{%- endfor %}

struct ResTabStorageAutoGen {
    int Load(const std::string & dir);

public:
    ///////////////////tables////////////////////
{% for df in defs %}
{%- if df.type == 'table' %}
    {{package}}::TB{{df.name}}Desc   m_tb{{df.name}}; //{{df.cn}}//{{df.desc}}
{%- endif %}
{%- endfor %}   
};



#endif
