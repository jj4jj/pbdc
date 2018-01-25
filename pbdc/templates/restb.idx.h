#ifndef _RES_TAB_{{package}}_INDEX_AUTO_GEN_H_
#define _RES_TAB_{{package}}_INDEX_AUTO_GEN_H_
{{declare}}
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

//pre declaration
class ResTabStorageBase;
{% if package %}
namespace {{package}} {
{%-for df in defs %}
{%-if df.type == 'table' %}
    class TB{{df.name}}Desc;
    class {{df.name}}Desc;
{%- endif %}
{%- endfor %}
};
{%-endif%}

class  ResTabIndexBase {
public:    
    int         Build();
    void        Clean();

protected:
    virtual     int         OnBuild();
    virtual     void        OnClean();

public:
{%for df in defs %}
{%-if df.type == 'table' %}
    //{{df.name}} accessor
    const {{package}}::TB{{df.name}}Desc &  {{df.name}}DescGetList() const;
    {%-if df.options.idx == 'id' %}
    const {{package}}::{{df.name}}Desc *  {{df.name}}DescGetBy{{df.pkeys[0].n|camel_style_name}}(int32_t {{df.pkeys[0].n}}) const;
    {%-else%}
    //for tree and hash map   
    struct {{df.name}}Key {
        {%-for fd in df.pkeys %}
        {{fd.t}}    {{fd.n}};
        {%-endfor%}
    public:
        //for std::equal_to , std::less, Hash
        bool operator == (const {{df.name}}Key & rhs) const {
            return (
        {%-for fd in df.pkeys %}
                this->{{fd.n}} == rhs.{{fd.n}} &&
        {%-endfor%}
                true );
        }
        bool operator < (const {{df.name}}Key & rhs) const {
            {%for fd in df.pkeys %}
            {%-if loop.index == 1%}if{%else%}else if{%endif%} (this->{{fd.n}} < rhs.{{fd.n}}) {
                return true;
            }
            else if (rhs.{{fd.n}} < this->{{fd.n}}){
                return false;
            }
            {%-endfor%}
            return false;
        }
        {%-if df.options.idx == 'hash' %}
        //for hash function
        struct Hash {
            size_t operator () (const {{df.name}}Key & key) const {
                {{df.name}}Key  tempk = key;
                uint64_t hashv = 0;
                {%-for fd in df.pkeys %}
                    {%- if fd.t == 'string' or fd.t == 'bytes' %}
                     auto strhvt_{{loop.index}} = std::hash<std::string>()(tempk.{{fd.n}});
                    {%- endif %}
                {%-endfor %}
                {%-for i in range(4) %}
                //{{i+1}} stage part hash value
                    {%-for fd in df.pkeys %}
                        {%- if i*loop.length*8+loop.index0*8 <= 56 %}
                hashv <<= 8;
                            {%-if fd.t == 'string' or fd.t == 'bytes' %}
                hashv += (strhvt_{{loop.index}} & 0xFF);
                strhvt_{{loop.index}} >>= 8;
                            {%-else%}
                hashv += (tempk.{{fd.n}} & 0xFF);
                tempk.{{fd.n}} >>= 8;                
                            {%-endif%}
                        {%-else%}
                //-skiped
                        {%- endif %}
                    {%-endfor %}
                {%-endfor%}

                return hashv;
            }
        }
        {%-endif%}
    };
    const {{package}}::{{df.name}}Desc *  {{df.name}}DescGetByKey(const {{df.name}}Key & key) const;
    {%-if df.options.idx == 'tree' %}
    const {{package}}::{{df.name}}Desc *  {{df.name}}DescUpperBoundByKey(const {{df.name}}Key & key) const;
    const {{package}}::{{df.name}}Desc *  {{df.name}}DescLowerBoundByKey(const {{df.name}}Key & key) const;
    const std::vector<{{package}}::{{df.name}}Desc *> & {{df.name}}DescRangeByKey(std::vector<{{package}}::{{df.name}}Desc *> vResult , const {{df.name}}Key & key) const;
    {%-endif%}
    {%-endif%}

{%- endif %}
{%- endfor %}

public:
    ResTabIndexBase(ResTabStorageBase * store):m_pStorage(store){}

private:
    ResTabStorageBase *  m_pStorage {nullptr};

public:
    //default indexes
{%- for df in defs %}
{%- if df.type == 'table' %}
    {%-if df.options.idx == 'id' %}
    std::vector<const {{package}}::{{df.name}}Desc *>    m_idx{{df.name}}; //index based id of {{df.name}}
    {%-elif df.options.idx == 'hash' %}
    typedef std::unordered_map<{{df.name}}Key, const {{package}}::{{df.name}}Desc *>  {{df.name}}HashMap;
    typedef {{df.name}}HashMap::iterator     {{df.name}}HashMapItr;
    {{df.name}}HashMap    m_idx{{df.name}}; //index based hastable of {{df.name}}
    {%-elif df.options.idx == 'tree' %}
    typedef std::map<{{df.name}}Key, const {{package}}::{{df.name}}Desc *>  {{df.name}}TreeMap;
    typedef {{df.name}}TreeMap::iterator     {{df.name}}TreeMapItr;
    {{df.name}}TreeMap    m_idx{{df.name}}; //index based treemap of {{df.name}}
    {%-else%}
    {%endif%}
{%- endif %}
{%- endfor %}   

};


#endif
