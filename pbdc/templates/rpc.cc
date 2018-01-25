{{declare}} 
#include "gslog.h"
#include "gmq/gmq.h"
#include "{{file}}.rpc.h"

{%-if package%}
namespace {{package}} {
{%-endif%}
{%-for df in defs%}
{%-if df.type == 'service' %}
const ::google::protobuf::Message * RpcGetBody(const {{df.name}} & msg){
    switch(msg.head().cmd()){
{%-for fd in df.fields %}
    case {{fd.t|rpc_cmd(df.name, df.options.cmd_pre)}}_REQ: //RPC:{{df.name}} REQUEST
    return   &msg.{{fd.t|rpc_name(df.name)}}().req();
    case {{fd.t|rpc_cmd(df.name, df.options.cmd_pre)}}_RES: //RPC:{{df.name}} RESPONSE
    return   &msg.{{fd.t|rpc_name(df.name)}}().res();
{%-endfor%}
    default:
    GLOG_ERR("not found the body for msg cmd:%d", msg.head().cmd());
    return NULL;
    }
}
::google::protobuf::Message * RpcMutableBody({{df.name}} & msg, {{df.name}}Cmd cmd){
    msg.mutable_head()->set_cmd(cmd);
    switch(cmd){
{%-for fd in df.fields %}
    case {{fd.t|rpc_cmd(df.name, df.options.cmd_pre)}}_REQ: //RPC:{{df.name}} REQUEST
    return msg.mutable_{{fd.t|rpc_name(df.name)}}()->mutable_req();
    case {{fd.t|rpc_cmd(df.name, df.options.cmd_pre)}}_RES: //RPC:{{df.name}} RESPONSE
    return msg.mutable_{{fd.t|rpc_name(df.name)}}()->mutable_res();
{%-endfor%}
    default:
    GLOG_ERR("not found the body for msg cmd:%d", msg.head().cmd());
    return NULL;
    }
}
{{df.name}} * RpcResponse(uint32_t dwGmqId, const {{df.name}} & request, MsgErrCode eRet){
    static {{df.name}} response; 
    response.mutable_head()->CopyFrom(request.head());
    response.mutable_head()->set_ret(eRet);
    auto resp = RpcMutableBody(response, {{df.name}}Cmd(request.head().cmd()+1));
    if(resp){
        GLOG_ERR("error request msg:%s to:%s ret:%d", GSPMSG(&request), GmqUtil::IdToStr(dwGmqId), eRet);
        return NULL;
    }
    return &response;
}
bool RpcCmdIsRequest({{df.name}}Cmd cmd){
    return (cmd % 2 == 1);
}
{%-if rpc_no_login|length > 0%}
bool RpcCmdIsNoLogin({{df.name}}Cmd cmd){
    switch(cmd){
    {%-for nologin in rpc_no_login%}
    case {{nologin}}_REQ:
    {%-endfor%}
    return true;
    default:
    return false;
    }
    return false;
}
{%-endif%}
{%-endif%}
{%-endfor%}


{%-if package%}
};
{%-endif%}

