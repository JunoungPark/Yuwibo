#pragma once
#pragma warning(push)
#pragma warning(disable: 4191)
#include <openssl/core_dispatch.h>
#pragma warning(pop)

# include <openssl/ssl.h>
# include <openssl/err.h>

struct CTXDeleter
{
    void operator()(SSL_CTX* ptr) const
    {
        if (ptr)
            SSL_CTX_free(ptr);
    }
};

struct SSLDeleter
{
    void operator()(SSL* ptr) const
    {
        if (ptr)
        {
            int Ret = SSL_shutdown(ptr);

            // 반환 값에 따라 적절히 처리합니다.
            if (Ret == 0)
            {
                // 1차 종료 성공, 2차 종료를 위해 다시 호출
                SSL_shutdown(ptr);
            }
            SSL_free(ptr);
        }
    }
};

class YUWIBO_API TLS
{
public:
    TLS();
    bool InitOpenSSL();

    void SetSocket(FSocket* _Socket) { Socket = _Socket; }

    size_t GetRecvText(Buffer& buf, int& err);
    int GetSecretText(Buffer& buf);

    void PrintSSLErr(const char* prefix);

    SSL* GetSSL() { return Ssl.Get(); }
    BIO* GetRBIO() { return Rbio; }
    BIO* GetWBIO() { return Wbio; }

    bool GetHandshaking() { return bHandshakeComplete; }
    void SetHandshaking(bool IsTrue) { bHandshakeComplete = IsTrue; }
private: 

    // OpenSSL objects
   TUniquePtr<SSL_CTX, CTXDeleter> SslCtx;
   TUniquePtr<SSL, SSLDeleter> Ssl;
   BIO* Rbio; // network -> SSL (encrypted in)
   BIO* Wbio; // SSL -> network (encrypted out)

    FSocket* Socket;

    bool bHandshakeComplete = true;

};
