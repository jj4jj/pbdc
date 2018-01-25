#ifndef _XDB_AGENT_{{file}}_H_
#define _XDB_AGENT_{{file}}_H_
{{declare}}

#include "dbagent.h"

typedef std::function<void(MsgErrCode ret, const ::google::protobuf::Message * pDbData, bool bInsert)> GetOrInsertFuture;
typedef std::function<void(MsgErrCode ret, uint64_t version)> UpdateFuture;
typedef std::function<void(MsgErrCode ret)> InsertFuture;
typedef std::function<void(MsgErrCode ret)> RemoveFuture;
typedef std::function<void(MsgErrCode ret, const ::google::protobuf::Message * pDbData, uint64_t version)> Getuture;

template<>
struct DBAgent<RoleDB> {
    MsgErrCode GetOrInsert({{df.name}}KeyType key, const {{df.name}} & init_data, GetOrInsertFuture future);
    MsgErrCode Update(const {{df.name}} & data, UpdateFuture future, bool bForce);
    MsgErrCode Insert(const {{df.name}} & data, InsertFuture future);
    MsgErrCode Remove(const {{df.name}}CxKeyType & key, RemoveFuture future);
    MsgErrCode Get(const {{df.name}}CxKeyType & key, GetFuture future);
};




#endif




