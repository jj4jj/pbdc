#!/bin/python
#-*- coding:utf-8 -*-
from __future__ import unicode_literals
from pbdc.pbdc import pdMsg,pdEnum,pdA,pdE,pdPackage,pdImport,pdEnd,pdRpc,pdReq,pdRes,pdFile,pdService


pdFile('cs')
pdPackage('u2')
pdImport('comm')
pdImport('res')


pdMsg('CSMsgHead')
pdE(t='MsgErrCode', n='ret')
pdE(t='string', n='ud')
pdE(t='uint32', n='time')
pdE(t='uint32', n='version')
pdEnd()
    
    
pdRpc('CSMsgLogin')
pdReq(t='int32', n='reconnect')
pdRes(t='CSRole', n='role')
pdEnd()


pdRpc('CSMsgRoleSync')
pdRes(t='CSRoleSyncType', n='sync_type')
pdRes(t='CSRole', n='role')
pdEnd()


pdService('CSMsg', cmd_pre='CS_CMD_', cmd_max=400)
pdE('CSMsgLogin', cn='login')
pdE('CSMsgRoleSync', cn='role sync')
pdEnd()


pdEnd()
