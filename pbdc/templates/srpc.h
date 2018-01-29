#ifndef _SRPC_UTIL_{{file}}_H_
#define _SRPC_UTIL_{{file}}_H_
{{declare}}
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
    bool RpcCmdIsRequest({{df.name}}Cmd cmd);
{%-endif%}
{%-endfor%}
{%-if package%}
};
{%-endif%}

//rpc service method prototyp declaration
{%-for df in defs%}
{%-if df.type == 'rpc' %}
namespace {{df.name|rpc_svc_name}}Svc {
    {{package}}::MsgErrCode   {{df.name|rpc_method_name}}(uint32_t dwFromGmqId, const {{package}}::{{df.name}}Req * p{{df.name|rpc_method_name}}Req, {{package}}::{{df.name}}Res * p{{df.name|rpc_method_name}}Res);
};
{%-endif%}
{%-endfor%}

#endif
