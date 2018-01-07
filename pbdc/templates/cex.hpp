//This file is generated auto by pbdc library , dont edit it directly .
//Any problem when using it , please contact mail:hex@gamesci.com.cn 
#ifndef _PB_{{file}}_CEX_AUTO_GEN_H_
#define _PB_{{file}}_CEX_AUTO_GEN_H_
{%for pkg in imports%}
#include "{{pkg}}.cex.hpp"
{%-endfor%}
#include "{{file}}.pb.h"
#include "pbdcex/pbdcex.core.hpp"
{%if package %}
namespace {{package}} {
{%-endif%}
{%for df in cex_defs %}
typedef struct tag{{df.name}} {
    //members
    {%-for fd in df.fields %}
    {{fd|cex_type}}    {{fd|cex_name}}; //fd.cn //fd.desc
    {%-endfor%}

    //construct
    void   construct(){
        if(sizeof(*this) < 4*1024){
            memset(this, 0, sizeof(*this));
        }
        else {
            {%-for fd in df.fields%}
            {%-if fd.repeat%}
            {{fd|cex_name}}.construct();//array construct
            {%-elif fd|cex_is_msg %}
            {{fd|cex_name}}.construct();//msg construct
            {%-elif fd.t == 'bool'%}
            {{fd|cex_name}}=false;
            {%-elif fd|cex_is_num%}
            {{fd|cex_name}}=static_cast<{{fd|cex_type}}>(0); //num
            {%-elif fd.t == 'string' or fd.t == 'bytes'%}
            {{fd|cex_name}}.construct(); //string
            {%-elif fd|cex_is_enum%}
            {{fd|cex_name}}={{fd.t}}_MIN; //enum
            {%-else%}
            static_assert(false, "error field type '{{fd.t}}' in cex define")
            {%-endif%}
            {%-endfor%}
        }
    }

    //pack and unpack
    int     CheckFrom(const {{df.name}} & msgfrom_){
        int ret = 0;
        {%-for fd in df.fields %}
        {%-if fd.repeat %}
        if ( msgfrom_.{{ fd.n }}_size() > {{ fd.count }} ){
            return __LINE__;//error for array
        }
        for (size_t i = 0; i < (size_t)msgfrom_.{{ fd.n }}_size(); ++i){
            {%-if fd|cex_is_msg %}
            ret = this->{{fd|cex_name}}.list[i].CheckFrom(msgfrom_.{{fd.n}}(i));
            if (ret){
                return ret;
            }
            {%-elif fd.t == 'bytes' %}
            if((msgfrom_.{{fd.n}}(i).length() > {{fd.length}}){
                return __LINE__;
            }
            {%-elif fd.t == 'string' %}
            if(msgfrom_.{{ fd.n }}(i).length() > {{ fd.length }}){
                return __LINE__;
            }
            {%-endif%}
        }
        {%-else%}
        {%-if fd|cex_is_msg%}
            return this->{{fd|cex_name}}.CheckFrom(msgfrom_.{{fd.n}}());
        {%-elif fd.t == 'bytes' or fd.t == 'string' %}
        if(msgfrom_.{{ fd.n }}().length() > {{ fd.length }}){
            return __LINE__;
        }
        {%-else %}
        this->{{fd|cex_name}} = msgfrom_.{{fd.n}}();
        {%-endif%}
        {%-endif%}
        {%-endfor%}
        return ret;
    }
    void     From(const {{df.name}} & msgfrom_,  bool bIgnoreEx = false){
        {%-for fd in df.fields %}
        {%-if fd.repeat %}
        this->{{fd|cex_is_msg}}.count=0;
        for (size_t i = 0; i < (size_t)msgfrom_.{{ fd.n }}_size() && i < {{ fd.count }}; ++i, ++(this->{{ fd|cex_name }}.count)){
            {%-if fd|cex_is_msg %}
            this->{{fd|cex_is_msg}}.list[i].From(msgfrom_.{{fd.n}}(i), bIgnoreEx);
            {%-elif fd.t == 'bytes' %}
            assert(bIgnoreEx || msgfrom_.{{fd.n}}(i).length() <= {{fd.length}});
            this->{{ fd|cex_name }}.list[i].assign(msgfrom_.{{ fd.n }}(i));
            {%-elif fd.t == 'string' %}
            assert(bIgnoreEx || msgfrom_.{{ fd.n }}(i).length() < {{ fd.length }});
            this->{{ fd|cex_name }}.list[i].assign(msgfrom_.{{ fd.n }}(i).data());
            {%-else%}
            this->{{fd|cex_is_msg}}.list[i] = msgfrom_.{{fd.n}}(i);
            {%-endif%}
        }
        {%-else%}
        {%-if fd|cex_is_msg%}
        this->{{fd|cex_name}}.From(msgfrom_.{{fd.n}}());
        {%-elif fd.t == 'bytes' %}
        assert(msgfrom_.{{ fd.n }}().length() <= {{ fd.length }});
        this->{{fd|cex_name}}.assign(msgfrom_.{{fd.n}}());
        {%-elif fd.t == 'string' %}
        assert(msgfrom_.{{ fd.n }}().length() < {{ fd.length }});
        this->{{fd|cex_name}}.assign(msgfrom_.{{fd.n}}().data());
        {%-else %}
        this->{{fd|cex_name}} = msgfrom_.{{fd.n}}();
        {%-endif%}
        {%-endif%}
        {%-endfor%}
    }
    void    To({{df.name}} & msgto_ ) const {
        msgto_.Clear();
        {%-for fd in df.fields%}
        {%-if fd.repeat%}
        for (size_t i = 0;i < (size_t){{fd.count}}; ++i){
            {%-if fd|cex_is_msg %}
            this->{{fd|cex_name}}.list[i].To(*msgto_.add_{{fd.n}}());
            {%-elif fd.t == 'bytes' %}
            msgto_.add_{{fd.n}}(this->{{fd|cex_name}}.list[i].data, this->{{fd|cex_name}}.list[i].length);
            {%-elif fd.t == 'string' %}
            msgto_.add_{{fd.n}}(this->{{fd|cex_name}}.list[i].data);
            {%-else%}
            msgto_.add_{{fd.n}}(this->{{fd|cex_name}}.list[i]);
            {%-endif%}
        }
        {%-else%}
        {%-if fd|cex_is_msg %}
        this->{{fd|cex_name}}.To(*msgto_.mutable_{{fd.n}}());
        {%-elif fd.t == 'bytes' %}
        msgto_.set_{{fd.n}}(this->{{fd|cex_name}}.data, this->{{fd|cex_name}}.length);
        {%-elif fd.t == 'string' %}
        msgto_.set_{{fd.n}}(this->{{fd|cex_name}}.data);
        {%-else%}
        msgto_.set_{{fd.n}}(this->{{fd|cex_name}});
        {%-endif %}
        {%-endif %}
        {%-endfor %}
    }

    //differ and patcher
    void     Diff(const {{ df.name }}Cx & orgv , {{ df.name }} & updates) const {
        updates.Clear();
        {%-for fd in df.fields %}
        {%-if fd.repeat %}
        {//block of checking array differ
            decltype(this->{{fd|cex_name}}.count) i = 0;
            for (i = 0;i < this->{{fd|cex_name}}.count; ++i) {
                auto upd_ = updates.mutable_{{fd.n}}()->Add();
                {%-if fd|cex_is_msg %}
                if (i < orgv.{{fd|cex_name}}.count) {
                    this->{{fd|cex_name}}.list[i].Diff(orgv.{{fd|cex_name}}.list[i], *upd_);
                }
                else {
                    this->{{fd|cex_name}}.list[i].convto(*upd_);
                }
                {%-elif fd.t == 'string' %}
                upd_->assign(this->{{fd|cex_name}}.list[i].data);
                {%-elif fd.t == 'bytes' %}
                upd_->assign((const char*)this->{{fd|cex_name}}.list[i].data, this->{{fd|cex_name}}.list[i].length);
                {%-else%}
                *upd_ = this->{{fd|cex_name}}.list[i];
                {%-endif%}
            }            
        }//end with array diff block 
        {%-elif fd|cex_is_msg %}
        this->{{fd|cex_name}}.Diff(orgv.{{fd|cex_name}}, *updates.mutable_{{ fd.n }}());
        {%-else%}
        {%-if fd.required %}
        {%-if fd.t == 'string' %}
        updates.set_{{ fd.n }}(this->{{ fd|cex_name }}.data);
        {%-elif fd.t == 'bytes' %}
        updates.set_{{ fd.n }}((char*)this->{{ fd|cex_name }}.data, this->{{ fd|cex_name }}.length);
        {%-else%}
        updates.set_{{ fd.n }}(this->{{ fd|cex_name }});
        {%-endif%}
        {%-else%}
        if (!(this->{{fd|cex_name}} == orgv.{{fd|cex_name}})) {
            {%-if fd.t == 'string' %}
            updates.set_{{ fd.n }}(this->{{ fd|cex_name }}.data);
            {%-elif fd.t == 'bytes'%}
            updates.set_{{ fd.n }}((char*)this->{{ fd|cex_name }}.data, this->{{ fd|cex_name }}.length);
            {%-else%}
            updates.set_{{ fd.n }}(this->{{ fd|cex_name }});
            {%-endif%}
        }
        {%-endif%}
        {%-endif%}
        {%-endfor%}
    }
    void     Patch(const {{ df.name }} & updates) {
        {%-for fd in df.fields%}
        {%-if fd.repeat %}
        {//block of checking array patch
            static {{fd.cex_type}}    item_temp;
            for (int i = 0; i < updates.{{fd.n}}_size(); ++i) {
                if (i < (int)this->{{fd|cex_name}}.count) {
                    {%-if fd|cex_is_msg %}
                    this->{{fd|cex_name}}.list[i].Patch(updates.{{fd.n}}(i));    
                    {%-elif fd.t == 'string' %}
                    this->{{fd|cex_name}}.list[i].assign(updates.{{fd.n}}(i));
                    {%-elif fd.t == 'bytes' %}
                    this->{{fd|cex_name}}.list[i].assign(updates.{{fd.n}}(i));
                    {%-else%}
                    this->{{fd|cex_name}}.list[i] = updates.{{fd.n}}(i);
                    {%-endif%}
                }
                else {
                    {%-if fd|cex_is_msg %}
                    item_temp.confdrom(updates.{{fd.n}}(i));
                    {%-elif fd.t == 'string' %}
                    item_temp.assign(updates.{{fd.n}}(i));
                    {%-elif fd.t == 'bytes' %}
                    item_temp.assign(updates.{{fd.n}}(i));
                    {%-else%}
                    item_temp = updates.{{fd.n}}(i);
                    {%-endif%}
                    this->{{fd|cex_name}}.lappend(item_temp);
                }
            }//end for append or update
        }//end with array diff block 
        {%-elif fd|cex_is_msg %}
        this->{{fd|cex_name}}.Patch(updates.{{ fd.n }}());
        {%-else%}
        {%-if fd.required %}
        {%-if fd.t == 'string' %}
        this->{{ fd|cex_name }}.assign(updates.{{ fd.n }}());
        {%-elif fd.t == 'bytes'%}
        this->{{ fd|cex_name }}.assign(updates.{{ fd.n }}());
        {%-else%}
        this->{{ fd|cex_name }} = updates.{{ fd.n }}();
        {%-endif%}
        {%-else%}
        if (updates.has_{{fd.n}}()) {
            {%-if fd.t == 'string' %}
            this->{{ fd|cex_name }}.assign(updates.{{ fd.n }}());
            {%-elif fd.t == 'bytes' %}
            this->{{ fd|cex_name }}.assign(updates.{{ fd.n }}());
            {%-else%}
            this->{{ fd|cex_name }} = updates.{{ fd.n }}();
            {%-endif%}
        }
        {%-endif%}
        {%-endif%}
        {%-endfor%}
    }
    int     compare(const {{df.name}}Cx & rhs_) const {
        int cmp = 0;
        {%- for fd in df.pkeys %}
        {%- if fd.repeat %}
        cmp = this->{{fd|cex_name}}.compare(rhs_.{{fd|cex_name}});
        if(cmp){return cmp;}
        {%-elif fd|cex_is_msg%}
        cmp = this->{{fd|cex_name}}.compare(rhs_.{{fd|cex_name}});
        if(cmp){return cmp;}
        {%-else%}
        if (this->{{fd|cex_name}} < rhs_.{{fd|cex_name}}){
            return -1;
        }
        else if(this->{{fd|cex_name}} > rhs_.{{fd|cex_name}}){
            return 1;
        }
        {%-endif%}
        {%-endfor%}
        return cmp;
    }
    bool    operator == (const {{df.name}}Cx & rhs_) const {
        return this->compare(rhs_) == 0;
    }
    bool    operator < (const {{df.name}}Cx & rhs_) const {
        return this->compare(rhs_) < 0;
    }
    size_t Hash() const {
        {%- if df.pkeys|length <= 1 %}
        {%- for fd in df.pkeys%}
        {%- if fd.repeat or fd.t == 'bytes' or fd.t == 'string' %}
        return this->{{fd|cex_name}}.hash();
        {%- elif fd|cex_is_num or fd|cex_is_enum or fd.t == 'bool' %}
        return (size_t)(this->{{fd|cex_name}});
        {%- elif fd|cex_is_msg %}
        return this->{{fd|cex_name}}.Hash();
        {%-else%}
        static_assert(false, "error field type '{{fd.t, fd.n}}'in cex");
        {%-endif%}
        {%-endfor%}
        {%-else%}
        size_t avhs[] = {
        {%-for fd in df.pkeys %}
        {%-if fd.repeat or fd.t == 'bytes' or fd.t == 'string'%}
            this->{{fd|cex_name}}.hash()
        {%-elif fd|cex_is_num or fd|cex_is_enum or fd.t == 'bool' %}
            (size_t)(this->{{fd|cex_name}})
        {%-elif fd|cex_is_msg%}
            this->{{fd|cex_name}}.Hash()
        {%-else%}
            static_assert(false, "error field type '{{fd.t, fd.n}}'in cex");
        {%-endif%}
        {%-if loop.last%}{%else%},{%endif%}
        {%-endfor%}
        };
        return pbdcex::hash_code_merge_multi_value(avhs, {{df.pkeys|length}});
        {%-endif%}
    }

} {{df.name}}Cx;

{%-endfor%}



{%if package %}
};
{%-endif%}

#endif