{{declare}}
#ifndef _RES_TAB_{{package}}_VERIFY_AUTO_GEN_H_
#define _RES_TAB_{{package}}_VERIFY_AUTO_GEN_H_

class ResTabIndexBase;
class  ResTabVerifierBase {

public:
    void        Clean();
    int         Verify();

protected:
    virtual     void        OnClean();
    virtual     int         OnVerify();

public:
    ResTabVerifierBase(ResTabIndexBase * index):m_pIndex(index){}

protected:
    ResTabIndexBase *  m_pIndex {nullptr};
};


#endif
