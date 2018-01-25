#ifndef _RES_TAB_{{package}}_STORAGE_AUTO_GEN_H_
#define _RES_TAB_{{package}}_STORAGE_AUTO_GEN_H_
{{declare}}
#include "{{file}}.pb.h"
class ResTabStorageBase {

public:
    int      Load();
    void     Clean();

protected:
    virtual     void    OnClean();
    virtual     int     OnLoad();

public:
    ResTabStorageBase(const std::string & dir);

public:
    std::string     m_strLoadDir;
    ///////////////////tables////////////////////
{% for df in defs %}
{%- if df.type == 'table' %}
    {{package}}::TB{{df.name}}Desc   m_tb{{df.name}}; //{{df.cn}}//{{df.desc}}
{%- endif %}
{%- endfor %}   


};

#endif
