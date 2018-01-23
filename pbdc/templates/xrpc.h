#ifndef _XRPC_UTIL_{{file}}_H_
#define _XRPC_UTIL_{{file}}_H_
#include "{{file}}.pb.h"
{%-if package%}
namespace {{package}} {
{%-endif%}
{%-for df in defs%}
{%-if df.type == 'service' %}
    //{{df.name}}
    const ::google::protobuf::Message * RpcGetBody(const {{df.name}} & msg);
    ::google::protobuf::Message * RpcMutableBody({{df.name}} & msg, {{df.name}}Cmd cmd);
    {{df.name}} * RpcResponse(uint32_t dwGmqId, const {{df.name}} & request, MsgErrCode eRet);
    bool RpcIsRequest(const {{df.name}} & ssmsg);
{%-endif%}
{%endfor%}
{%-if package%}
};
{%-endif%}

#endif
