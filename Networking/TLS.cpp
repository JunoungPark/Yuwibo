#include "TLS.h"
#include "Sockets.h"
#include "Buffer.h"
#include "iostream"

TLS::TLS()
    : SslCtx(nullptr)
    , Ssl(nullptr)
    , Rbio(nullptr)
    , Wbio(nullptr)
    , bHandshakeComplete(true)
{
}

bool TLS::InitOpenSSL() {
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();

    auto raw_SslCtx = SSL_CTX_new(TLS_client_method());
    if (!raw_SslCtx)
    {
        UE_LOG(LogTemp, Log, TEXT("SSL_CTX Create Failed"));
        return false;
    }
    // Set secure defaults
    SSL_CTX_set_min_proto_version(raw_SslCtx, TLS1_3_VERSION);

    // Create SSL object
    auto raw_Ssl = SSL_new(raw_SslCtx);
    if (!raw_Ssl) {
        UE_LOG(LogTemp, Log, TEXT("SSL Create Failed"));
        return false;
    }
    // Create memory BIOs and attach
    Rbio = BIO_new(BIO_s_mem());
    Wbio = BIO_new(BIO_s_mem());
    SSL_set_bio(raw_Ssl, Rbio, Wbio); // SSL owns them now

    SSL_set_connect_state(raw_Ssl); // client mode

    SslCtx.Reset(raw_SslCtx);
    Ssl.Reset(raw_Ssl);

    UE_LOG(LogTemp, Log, TEXT("InitOpenSSL success: ssl=%p rbio=%p wbio=%p"), raw_Ssl, Rbio, Wbio);

    return true;
}


size_t TLS::GetRecvText(Buffer& buf, int& err)
{
    err = SSL_ERROR_NONE;

    while (buf.GetRemainingSize())
    {
        int n = SSL_read(Ssl.Get(), buf.GetWriteBuffer(), buf.GetRemainingSize());

        if (n > 0)
            buf.OnWrite(n);
        else
        {
            err = SSL_get_error(Ssl.Get(), n);

            break;
        }
    }

    return buf.GetStoredSize();
}

int TLS::GetSecretText(Buffer& buf)
{
    while (buf.GetRemainingSize())
    {
        int n = BIO_read(Wbio, buf.GetWriteBuffer(), buf.GetRemainingSize());

        if (n > 0)
            buf.OnWrite(n);
        else
            break;
    }

    return buf.GetStoredSize();
}

void TLS::PrintSSLErr(const char* prefix)
{
    std::cerr << prefix << " : ";
    unsigned long e;
    while ((e = ERR_get_error()) != 0)
    {
        char buf[256];
        ERR_error_string_n(e, buf, sizeof(buf));
        std::cerr << buf << " | ";
    }
    std::cerr << "\n";
}
