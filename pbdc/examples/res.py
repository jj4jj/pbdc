#!/bin/python
#-*- coding:utf-8 -*-
from __future__ import unicode_literals

from pbdc.pbdc import pdFile,pdMsg,pdEnum,pdA,pdE,pdPackage,pdImport,pdEnd,pdRpc,pdReq,pdRes,pdTab


pdFile('res')
pdPackage('u2')


pdEnum('ResConstValueDef', alias=True)
pdE(n='MAX_AWARD_CONF_ITEM_NUM', v='8', cn='max_award_conf_item')
pdEnd()

pdTab('LogicValue', id=True)
pdE(t='int32', n='value') 
pdEnd()

pdTab('Level', id=True, id_alias='level')
pdE(t='uint32', n='need_exp') 
pdEnd()


pdMsg('AwardItem')
pdE(t='AwardType', n='type')
pdE(t='int32', n='value')
pdE(t='int32', n='num')
pdEnd()


pdTab('Award', id=True)
pdA(t='AwardItem', n='item', count='MAX_AWARD_CONF_ITEM_NUM') 
pdEnd()


pdEnd(['pb2','res.s.h','res.s.c','res.i.h',])
