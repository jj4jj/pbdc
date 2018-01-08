#!/bin/python
#-*- coding:utf-8 -*-
from __future__ import unicode_literals
from jinja2 import Template, Environment, TemplateSyntaxError
import os
import sys

########################################################################################################
gtpl_env = Environment()
cex_ext_type_2_pb2type = {
    'uint8':'uint32',
    'uint16':'uint32',
    'int8':'int32',
    'int16':'int32',
}
cex_ext_type_max_value = {
    'uint8':255,
    'uint16':65535,
    'int8':128,
    'int16': 32768,
}
cex_ext_type_min_value = {
    'uint8':0,
    'uint16':0,
    'int8':-127,
    'int16': -32767,
}
cex_ext_types = cex_ext_type_2_pb2type.keys()
pb_int_types = ('uint32','uint64','int32','int64','fixed64')
pb_error_types = ('sfixed64','sfixed32','fixed32')
primitive_types_integer = set(list(cex_ext_types) + list(pb_int_types))
primitive_types_real = set(('float','double'))
primitive_types_number = primitive_types_integer | primitive_types_real
primitive_types = set(('string','bool','bytes')) | primitive_types_number
cex_primitive_types_2_cxx_types = {
    'uint8':'uint8_t',
    'int8':'int8_t',
    'uint16':'uint16_t',
    'int16':'int16_t',
    'uint32':'uint32_t',
    'int32':'int32_t',
    'uint64':'uint64_t',
    'int64':'int64_t',
    'float':'float',
    'double':'double',
    'bool':'bool',
    'string':'error type',
    'bytes':'error type',
    'fixed32':'uint32_t',
    'sfixed32':'int32_t',
    'fixed64':'uint64_t',
    'sfixed64':'int64_t',
}

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
    'cex': CodeGenerater('.cex.hpp','cex.hpp'),
    'rpc.h': CodeGenerater('.rpc.h', 'xrpc.h'),
    'rpc.c': CodeGenerater('.rpc.cc', 'xrpc.cc'),
    'db.h': CodeGenerater('.db.h', 'xdb.h'),
    'db.c': CodeGenerater('.db.cc', 'xdb.cc'),
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

def camel_style_name(name):
    rname = ''
    trans = True
    for ch in name:
        if ch == '_':
            trans = True
        else:
            if ch.islower() and trans:
                rname += ch.upper()
                trans = False
            else:
                rname += ch

    return rname

#CSMsgLoginQuery->LoginQuery->login_query
def rpc_name(entry_type_name, service_name):
    pos = entry_type_name.find(service_name)
    if pos == -1:
        raise Exception('rpc define error for pdE type do not have a service type prefix type:%s' % entry_type_name)
    return camel_style_trans(entry_type_name[pos+len(service_name):])


###################################################################################################
def rpc_cmd(entry_type_name, service_name, cmd_prefix):
    #print cmd_prefix
    rname = rpc_name(entry_type_name, service_name)
    return cmd_prefix + rname.upper()
    
def cex_is_num(field):
    return field['t'] in primitive_types_number

def cex_is_ext_primitive(field):
    return field['cex_type'] in cex_ext_types

def cex_ext_max_value(field):
    return cex_ext_type_max_value[field['cex_type']]

def cex_ext_min_value(field):
    return cex_ext_type_min_value[field['cex_type']]

def cex_is_msg(field):
    if field['t'] in primitive_types:
        return False
    tm = gtx.meta.get(field['t'], None)
    if tm is None:
        pt = gtx.pragma['type'].get(field['t'],None)
        if pt is None or pt == 'msg':
            return True
        return False
    else:
        if tm.type == 'msg' or tm.type == 'table':
            return True
        assert tm.type == 'enum', 'field type:%s is not a msg and not enum in cex  ref is error !'
        return False

def cex_is_enum(field):
    if field['t'] in primitive_types:
        return False
    tm = gtx.meta.get(field['t'], None)
    if tm is None:
        pt = gtx.pragma['type'].get(field['t'],None)        
        return pt == 'enum'
    else:
        return tm.type == 'enum'

def cex_name(field):
    return field['n']

def cex_type(field):
    mtype = field['t']
    if cex_is_msg(field):
        mtype = field['t']+'Cx'
    elif field['t'] == 'string':
        mtype = 'pbdcex::string_t<%s>' % field['length']
    elif field['t'] == 'bytes':
        mtype = 'pbdcex::bytes_t<%s>' % field['length']
    else:
        mtype = cex_primitive_types_2_cxx_types[field['cex_type']]
    ##################
    if field['repeat']:
        return 'pbdcex::array_t<%s,%s>' % (mtype, field['count'])
    return mtype

def cex_has_default_value(msgdef):
    for fd in msgdef.fields:
        if fd.get('v', 0) != 0:
            return True
    return False

def length(a):
    return len(a)

gtpl_env.filters={'rpc_name':rpc_name,
                  'rpc_cmd':rpc_cmd,
                  'camel_style_name':camel_style_name,
                  'cex_type': cex_type,
                  'cex_name': cex_name,
                  'cex_is_num': cex_is_num,
                  'cex_is_msg': cex_is_msg,
                  'cex_is_enum': cex_is_enum,
                  'cex_is_ext_primitive': cex_is_ext_primitive,
                  'cex_ext_min_value': cex_ext_min_value,
                  'cex_ext_max_value': cex_ext_max_value,
                  'cex_is_enum': cex_is_enum,
                  'cex_has_default_value': cex_has_default_value,
                  'length': length,
                  }
###################################################################################################


class DefStage(dict):
    def __init__(self, type, name, options={}):
        self.type = type
        self.name = name
        self.options = options
        self.fields = []
        self.req = []
        self.res = []
        self.pkeys = []
        ##################
        self.begin()

    def on_table_end(self):
        if self.options.get('limit_size', 0) == 0:
            self.options['limit_size'] = 1000*1000*10
        if self.options.get('id', None) is True:
            id_alias = self.options.get('id_alias', 'id')
            self.fields.insert(0, dict(t='int32',n=id_alias, required=True))
            self.options['ks'] = id_alias
            self.options['idx'] = 'id'
            if self.options.get('id_max', 0) == 0:
                self.options['id_max'] = 255
        else:
            if self.options.get('idx', None) is None:
                self.options['idx'] = 'list'                

        if self.options.get('ks',None) is None:
            self.options['ks'] = self.fields[0]['n']

        for k in self.options['ks'].split(','):
            for fd in self.fields:
                if k == fd['n']:
                    self.pkeys.append(fd)
                    break

    def on_msg_end(self):        
        if self.ks is None:
            self.ks = ','.join([fd['n'] for fd in self.fields])
            self.pkeys = self.fields
        else:
            fks = self.ks.split(',')
            self.pkeys = [fd for fd in self.fields if fd.n in fks ]
        assert len(self.pkeys) > 0,'error define for msg:%s property:"ks:%s"' % (self.name, self.ks)
        if self.options.get('cex',None) is True:
            gtx.cexs.append(self)

    def begin(self):
        hook = getattr(self, 'on_%s_begin' % self.type, None)
        if hook:
            hook()            

    def end(self):
        hook = getattr(self, 'on_%s_end' % self.type, None)
        if hook:
            hook()

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
        self.cexs = []
        self.pragma = {
            'type':{}
        }

    def __getattr__(self, attr):
        return self.get(attr, None)

    def __setattr__(self, attr, val):
        self[attr]=val

    def on_file_begin(self):
        for bkd in backends:
            ext = bkd.replace('.','_')
            hook = getattr(self, 'on_%s_begin' % ext,  None)
            if hook:
                hook()

    def on_file_end(self):
        for bkd in backends:
            ext = bkd.replace('.','_')
            hook = getattr(self, 'on_%s_end' % ext,  None)
            if hook:
                hook()

    def cex_check_msg_constraints(self, msgdef):
        for fd in msgdef.fields:
            if (fd['t'] == 'string' or fd['t'] == 'bytes') and fd.get('length', None) is None:
                raise Exception('cex define %s.%s property "length" is required !' % (msgdef.name, fd['n']))
            if fd['repeat'] and fd.get('repeat', None) is None:
                raise Exception('cex define %s.%s property "count" is required !' % (msgdef.name, fd['n']))
            ###


    def cex_build_type_reference(self):
        checked = set()
        tunkown = set()
        queue = [cex for cex in self.cexs]
        while len(queue) > 0:
            ct = queue.pop()
            if ct.name in checked:
                continue
            self.cex_check_msg_constraints(ct)
            checked.add(ct.name)
            ct.cex_refs = []
            for fd in ct.fields:
                if fd['t'] in primitive_types:
                    continue
                tr = self.meta.get(fd['t'], None)
                if tr is None:                    
                    tunkown.add(fd['t'])
                else:
                    if tr.type == 'enum':
                        continue
                    else:
                        if tr.type != 'msg':
                            raise Exception('error cex parsing for type def type "%s" "%s" , '\
                                            'must def by pdMsg or pdTab or pdEnum' % (tr.type, tr.name))
                        if fd['t'] in ct.cex_refs:
                            continue
                        ct.cex_refs.append(fd['t'])
                        queue.insert(0, tr)
        #
        return tunkown

    def cex_topology_sort(self):
        def find_unrefs(root, excepts, meta):
            if root.name in excepts:
                return []
            if len(set(root.cex_refs)-set(excepts)) == 0:
                return [root.name]
            res = []
            for cr in root.cex_refs:
                res.extend(find_unrefs(meta[cr], excepts, meta))
            return res
        ################################################
        stypes = []
        excepts = []
        while True:
            #find unrefs
            unref_types = []
            for df in self.cexs:
                unref_types.extend(find_unrefs(df, excepts, self.meta))
            if len(unref_types) == 0:
                break
            stypes.extend(unref_types)
            excepts.extend(unref_types)
        return stypes                


    def on_cex_end(self):
        #build a graph########################################
        self.cex_unknowns = self.cex_build_type_reference()
        for unk in self.cex_unknowns:
            unkt = self.pragma['type'].get(unk, None)
            if unkt is None:
                print 'WARNNING: cex reference type:"%s" is a extern type and not found a '\
                      'pragma info, assume it is message type' % (unk,)
        #######################################################
        stypes = self.cex_topology_sort()
        #print stypes, self.cex_unknowns
        self.cex_defs = []
        for st in stypes:            
            self.cex_defs.append(self.meta[st])            

########################################################################################################
gtx = Ctx()
def pdPragma(p, key, val=None):
    gtx.pragma[p][key] = val

def pdFile(name):
    gtx.reset(name)
    gtx.on_file_begin()

def pdImport(name):
    gtx.imports.append(name)

def pdPackage(name):
    gtx.package = name

def pdMsg(name, **kwargs):
    gtx.stack.append(DefStage('msg', name, kwargs))

def pdService(name, **kwargs):
    gtx.stack.append(DefStage('service', name, kwargs))

def pdConfig(name, **kwargs):
    kwargs['cex'] = True
    gtx.stack.append(DefStage('msg', name, kwargs))

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
    if kwargs.get('repeat',None) is None:
        kwargs['repeat'] = False
    if kwargs.get('required',None) is None:
        kwargs['required'] = False
    current_ctype = gtx.stack[-1].type
    if current_ctype == 'msg' or current_ctype == 'table':
        if(len(args) > 0):
            kwargs['t']=args[0]
        if(len(args) > 1):
            kwargs['n']=args[1]
    	check_keys(kwargs, ['n','t'])
        ###################################cex##############
        ft = kwargs['t']
        kwargs['cex_type'] = ft
        if ft in cex_ext_types:
            kwargs['t'] = cex_ext_type_2_pb2type[ft]

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
    dt = kwargs.get('t',None)
    assert (dt not in pb_error_types),'pbdc deprecate protobuf using this type "%s" the field.' % (dt, str(kwargs))
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
            print 'generating [%s@%s]' % (cg, ctx.file),os.path.abspath(path),'...'
            data = gtpl_env.from_string(open(tp).read()).render(ctx).encode('utf-8')
        except TemplateSyntaxError, e:
            ##exception jinja2.TemplateSyntaxError(message, lineno, name=None, filename=None)
            raise Exception('syntax error for def for "#%d ---> %s" code gen type:%s' % (e.lineno, e.message, cg))
        except Exception,e:
            raise Exception('python syntax error for "%s" code gen type : "%s:%s"' % (str(e), ctx.file, cg))
        open(path,'wb+').write(data)

def getopt(k):
    if len(sys.argv) < 2:
        return None
    kvopts = [opt.strip().split('=') for opt in sys.argv[1:]]
    for kv in kvopts:
        if len(kv) == 2 and (kv[0] == k or kv[0] == '--%s' % k):
            return kv[1]
    return None


def pdEnd(codegen=['pb2'], outdir=None):
    assert len(gtx.stack) >= 0,'defination not match for end'
    if outdir is None:
        outdir = getopt('outdir')
    if outdir is None:
        outdir = './'
    if len(gtx.stack) == 0:
        gtx.on_file_end()
        #print gtx
        assert len(gtx.file) > 0,'end of file generation error . did you forget "pdFile(...)" ?'
        pdGenerate(gtx, codegen, outdir)
        gtx.reset()
    else:
        ddef = gtx.stack.pop()
        ddef.end()
        gtx.meta[ddef.name] = ddef
        gtx.defs.append(ddef)

