#!/bin/python
#-*- coding:utf-8 -*-
from __future__ import unicode_literals
from jinja2 import Template, Environment
import os

########################################################################################################
class CodeGenerater(object):
    def __init__(self, ext_name, template):
        self.ext_name=ext_name
        self.template=template
template_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)),'templates')
backends = {
    'pb2': CodeGenerater('.proto','pb2.proto'),
    'res.s.h': CodeGenerater('.store.h','restb.store.h'),
    'res.s.c': CodeGenerater('.store.cc','restb.store.cc'),
    'res.i.h': CodeGenerater('.idx.h','restb.idx.h'),
    'res.i.c': CodeGenerater('.idx.cc','restb.idx.cc'),
    'res.v.h': CodeGenerater('.verify.h','restb.verify.h'),
    'res.v.c': CodeGenerater('.verify.cc','restb.verify.cc'),
    'mm': CodeGenerater('.cex.hpp','cex.hpp'),
}
########################################################################################################
def camel_style_trans(cname):
    pre_is_lower = False
    rname = ''
    for ch in cname:
        if ch.isupper():
            if pre_is_lower:
                rname += '_'
                pre_is_lower = False
            rname += ch.lower()
        else:
            rname += ch 
            pre_is_lower = True
    return rname


#CSMsgLoginQuery->LoginQuery->login_query
def rpc_name(entry_type_name, service_name):
    pos = entry_type_name.find(service_name)
    if pos == -1:
        raise Exception('rpc define error for pdE type do not have a service type prefix type:%s' % entry_type_name)
    return camel_style_trans(entry_type_name[pos+len(service_name):])

#CSMsgLoginQuery->LOGIN_QUERY
def rpc_cmd(entry_type_name, service_name, cmd_prefix):
    #print cmd_prefix
    rname = rpc_name(entry_type_name, service_name)
    return cmd_prefix + rname.upper()
    
tpenv = Environment()
tpenv.filters={'rpc_name':rpc_name,'rpc_cmd':rpc_cmd}

class DefStage(dict):
    def __init__(self, type, name, options={}):
        self.type = type
        self.name = name
        self.options = options
        self.fields = []
        self.req = []
        self.res = []

    def __getattr__(self, attr):
        return self.get(attr, None)

    def __setattr__(self, attr, val):
        self[attr]=val


class Ctx(dict):
    def __init__(self):
        self.reset()

    def reset(self, name=''):
        self.file = name
        self.imports = []
        self.package = ''
        self.stack = []
        self.meta = {}
        self.defs = []

    def __getattr__(self, attr):
        return self.get(attr, None)

    def __setattr__(self, attr, val):
        self[attr]=val



########################################################################################################
gtx = Ctx()
def pdFile(name):
    gtx.reset(name)

def pdImport(name):
    gtx.imports.append(name)

def pdPackage(name):
    gtx.package = name

def pdMsg(name, **kwargs):
    gtx.stack.append(DefStage('msg', name, kwargs))

def pdService(name, **kwargs):
    gtx.stack.append(DefStage('service', name, kwargs))

def pdEnum(name, **kwargs):
    #print gtx.stack
    gtx.stack.append(DefStage('enum', name, kwargs))

def pdTab(name, **kwargs):
    gtx.stack.append(DefStage('table', name, kwargs))

def pdRpc(name, **kwargs):
    gtx.stack.append(DefStage('rpc', name, kwargs))


def check_keys(kwargs, keys=[]):
    for k in keys:
        v = kwargs.get(k, None)
        if v is None:
            raise Exception('not found the key:"%s" in define context:"%s:%s"...' % (k, str(gtx.stack[-1].name), str(gtx.stack[-1].type)))

def pdRes(**kwargs):
    if 'rpc' != gtx.stack[-1].type:
        raise Exception('def response should be in a Rpc context (need a pdRpc before)')
    check_keys(kwargs, ['t','n'])
    gtx.stack[-1].res.append(kwargs)

def pdReq(**kwargs):
    if 'rpc' != gtx.stack[-1].type:
        raise Exception('def request should be in a Rpc context (need a pdRpc before)')
    check_keys(kwargs, ['t','n'])
    gtx.stack[-1].req.append(kwargs)

def pdE(*args, **kwargs):
    assert len(gtx.stack) > 0,'define field (entry) not in a context'
    current_ctype = gtx.stack[-1].type
    if current_ctype == 'msg' or current_ctype == 'table':
        if(len(args) > 0):
            kwargs['t']=args[0]
        if(len(args) > 1):
            kwargs['n']=args[1]
    	check_keys(kwargs, ['n','t'])
    if current_ctype == 'service':
        if(len(args) > 0):
            kwargs['t']=args[0]
        kwargs.pop('n',None)
    	check_keys(kwargs, ['t'])
    if current_ctype == 'enum':
        if(len(args) > 0):
            kwargs['n']=args[0]
        if(len(args) > 1):
            kwargs['v']=args[1]
    	check_keys(kwargs, ['n','v'])
    gtx.stack[-1].fields.append(kwargs)

def pdA(*args, **kwargs):
    assert len(gtx.stack) > 0,'define field (entry) not in a context'
    kwargs['repeat'] = True
    pdE(*args, **kwargs)


def pdGenerate(ctx, codegen, outdir):
    for cg in codegen:
        assert cg in backends, 'not found the codgerator for:'+cg
        tp = os.path.join(template_dir,backends[cg].template)
        ext_name = backends[cg].ext_name
        path = os.path.join(outdir, ctx.file+ext_name)
        try:
            data = tpenv.from_string(open(tp).read()).render(ctx).encode('utf-8')
        except Exception,e:
            raise Exception('syntax error for proto def for "%s" code gen type:%s' % (str(e), cg))
        open(path,'wb+').write(data)


def pdEnd(codegen=['pb2'], outdir='./gen'):
    assert len(gtx.stack) >= 0,'defination not match for end'
    if len(gtx.stack) == 0:
        #print gtx
        assert len(gtx.file) > 0,'end of file generation error . did you forget "pdFile(...)" ?'
        pdGenerate(gtx, codegen, outdir)
        gtx.reset()
    else:
        ddef = gtx.stack.pop()
        gtx.meta[ddef.name] = ddef
        gtx.defs.append(ddef)

