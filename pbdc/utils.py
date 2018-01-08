


#proto generate objects
def pygen_cmake_cmds(cdir, files, pygen_incs=[], pygen=True, outdir='./', target=None, build=False):
    objs = []
    cdir = os.path.abspath(cdir)
    outs = []
    pincs = ' -I'.join(pygen_incs)
    if pincs != '':
        pincs = '-I'+pincs
    for bname in files:
        outf = '%s/gen/%s.pb.cc' % (cdir, bname)
        outs.append(outf)
        obj = dict(out=outf,
                dep='%s/gen/%s.proto' % (cdir, bname),
                cmd='{{protoc}} %s/gen/%s.proto -I%s/gen %s --cpp_out=%s' % (cdir, bname, cdir, pincs, outdir),
                build=build)
        objs.append(obj)
        if pygen:
            outf = '%s/gen/%s.proto' % (cdir, bname)
            outs.append(outf)
            obj = dict(out=outf,
                    dep='%s/%s.py' % (cdir, bname),
                    cmd='cd %s && python %s.py --out_dir=%s' % (cdir, bname, outdir),
                    build=build)
            objs.append(obj)
    if target:
        obj = dict(name=target,force=True,dep=' '.join(outs),srcs='', cmd='')
        objs.append(obj)
    return objs
