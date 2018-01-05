#!/bin/python
#-*- coding:utf-8 -*-
from __future__ import unicode_literals
from pbdc.pbdc import pdMsg,pdEnum,pdA,pdE,pdPackage,pdImport,pdEnd,pdRpc,pdReq,pdRes,pdFile


pdFile('comm')
pdPackage('u2')
pdImport('res')

pdEnum('MsgErrCode')
pdE(n='ERR_SUCCESS', v='0', cn='sucess')
pdE(n='ERR_LOGIN_ERR', v='1', cn=u'login error')
pdEnd()


pdEnum('CommConstValueDef', alias=True)
pdE(n='MAX_ROLE_NAME_LEN', v='25', cn='max role name len')
pdE(n='MAX_ROLE_FRIEND_NUM', v='100', cn='max role friend num')
pdEnd()


pdEnum('CSRoleSyncType')
pdE(n='CS_ROLE_SYNC_LOGIN', v='0')
pdEnd()


pdMsg('RoleBase')
pdE(t='fixed64', n='roleid')
pdE(t='string', n='name', length='MAX_ROLE_NAME_LEN')
pdEnd()


pdMsg('RoleDataCS')
pdE(t='RoleBase', n='base')
pdEnd()


pdMsg('CSRole')
pdE(t='RoleDataCS',n='cs')
pdEnd()


pdEnd()
