#ifndef _CRPC_UTIL_{{file}}_H_
#define _CRPC_UTIL_{{file}}_H_
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
    bool RpcCmdIsNoLogin({{df.name}}Cmd cmd);
    bool RpcCmdIsRequest({{df.name}}Cmd cmd);
{%-endif%}
{%-endfor%}
{%-if package%}
};
{%-endif%}

//rpc service method prototyp declaration
class Player;
class Session;
{%-for df in defs%}
{%-if df.type == 'rpc' %}
namespace {{df.name|rpc_svc_name}}Svc {
    {%if df.options.no_login%}
    {{package}}::MsgErrCode   {{df.name|rpc_method_name}}(Session * pSession, const {{package}}::{{df.name}}Req * {{df.name|rpc_method_name}});
    {%else%}
    {{package}}::MsgErrCode   {{df.name|rpc_method_name}}(Player * pPlayer, const {{package}}::{{df.name}}Req * {{df.name|rpc_method_name}});
    {%endif%}
};
{%-endif%}
{%-endfor%}

#endif
