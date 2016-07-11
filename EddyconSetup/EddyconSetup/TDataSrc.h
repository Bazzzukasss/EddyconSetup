#ifndef TDATASRC_H
#define TDATASRC_H
template <typename T>
class TDataSrc{
public:
    TDataSrc();
    ~TDataSrc();
private:
    T*        Buf;
    T*        BegPtr;
    T*        CurPtr;
    T*        WrPtr;
    T*        RdPtr;
    T*        SwRdPtr;
    T*        ASDPtr;
    T*        RvtpRdPtr;
    T*        AcRdPtr;
};

#endif // TDATASRC_H
