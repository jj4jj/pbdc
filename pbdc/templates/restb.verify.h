//This file is generated auto by pbdc library , dont edit it directly .
//Any problem when using it , please contact mail:hex@gamesci.com.cn 

#ifndef _RES_TAB_{{package}}_VERIFY_AUTO_GEN_H_
#define _RES_TAB_{{package}}_VERIFY_AUTO_GEN_H_

class ResTabIndexBase;
class  ResTabVerifierBase {

public:    
    int         Verify();

protected:
    virtual     int         OnVerify();

public:
    ResTabVerifierBase(ResTabIndexBase * index):m_pIndex(index){}

protected:
    ResTabIndexBase *  m_pIndex {nullptr};
};


#endif
