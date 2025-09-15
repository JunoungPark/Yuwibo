// -----------------------------
// File: Private/TLSConnection.cpp
// -----------------------------
#include "FTLSConnection.h"
#include "Misc/ScopeLock.h"
#include "Containers/Queue.h"
#include "HAL/PlatformProcess.h"
#include "Buffer.h"
#include "FPacketHandler.h"

#undef SetPort

FTLSConnection::FTLSConnection(TQueue<TUniquePtr<Buffer>>& _RecvBufQueue, TLS& tls)
    : RecvBufQueue(_RecvBufQueue)
    , Tls(tls)
    , Socket(nullptr)
    , SocketSubsystem(nullptr)
    , Thread(nullptr)
    , WakeEvent(nullptr)
    , IsRunning(true)
{
}

FTLSConnection::~FTLSConnection()
{
    Close();
}

bool FTLSConnection::Init()
{
    if (!Tls.InitOpenSSL())return false;

    WakeEvent = FPlatformProcess::GetSynchEventFromPool(false);
    return true;
}

uint32 FTLSConnection::Run()
{
        // Wait for data to send or socket activity
        // Timeout so we can poll recv even without sends 
        // 1) handle recv
    while (IsRunning)
    {
        WakeEvent->Wait(0.02f);

        int32 BytesRead = 0;

        auto RecvBuf = MakeUnique<Buffer>();

        if (Socket->Recv(RecvBuf->GetBuffer(), RecvBuf->GetBufferSize(), BytesRead))
        {
            if (BytesRead > 0)
            {
                RecvBuf->OnWrite(BytesRead);

                RecvBufQueue.Enqueue(MoveTemp(RecvBuf));
            }
        }
    }
    return 0;

}

void FTLSConnection::Stop()
{
    UE_LOG(LogTemp, Log, TEXT("Stop"));
    IsRunning = false;
    if (WakeEvent) WakeEvent->Trigger();
}

void FTLSConnection::Exit()
{
}

bool FTLSConnection::Connect(const FString& Host, int32 Port)
{
    if (!NonBlockingConnect(Host, Port))
    {
        UE_LOG(LogTemp, Error, TEXT("Socket connect failed"));
        return false;
    }

    IsConnect = true;
    UE_LOG(LogTemp, Log, TEXT("Set Is Connect"));
    // create thread and start run
    Thread = FRunnableThread::Create(this, TEXT("TLSConnectionThread"));

    TLSHandshake();

    return Thread != nullptr;
}

bool FTLSConnection::TLSHandshake()
{
    UE_LOG(LogTemp, Log, TEXT("Handshake"));

    int ret = SSL_do_handshake(Tls.GetSSL());

    FlushTLSWriteBuffer();

    if (ret == 1)
    {
        // 핸드셰이크 성공
        UE_LOG(LogTemp, Log, TEXT("TLS Handshake complete"));
        Tls.SetHandshaking(false);
        return true;
    }

    int err = SSL_get_error(Tls.GetSSL(), ret);

    if (err == SSL_ERROR_WANT_READ);
    else if (err == SSL_ERROR_WANT_WRITE)
    {
        FlushTLSWriteBuffer();
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Handshake Failed : %d"), err);
        ERR_print_errors_fp(stderr);
        Close();
    }

    return false;
}

bool FTLSConnection::FlushTLSWriteBuffer()
{
    Buffer buf;
    auto n = Tls.GetSecretText(buf);

    if (n <= 0) return false;

    size_t len = buf.GetStoredSize();

    UE_LOG(LogTemp, Log, TEXT("SendData Len %d"),len);

    while (len)
    {
        UE_LOG(LogTemp, Log, TEXT("FlushTLSWriteBuffer"));

        int32 BytesSent = 0;

        if (!Socket->Send(buf.GetReadBuffer(), len, BytesSent))
        {
            UE_LOG(LogTemp, Log, TEXT("SocketSend Failed"));
            break;
        }

        buf.OnRead(BytesSent);

        len = buf.GetStoredSize();
    }

    return true;
}

bool FTLSConnection::NonBlockingConnect(const FString& Host, int32 Port)
{
    ISocketSubsystem* SocketSub = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    if (!SocketSub) return false;

    Socket = SocketSub->CreateSocket(NAME_Stream, TEXT("tls-client"), false);
    if (!Socket) return false;

    Tls.SetSocket(Socket);

    Socket->SetNonBlocking(true);
    Socket->SetReuseAddr(true);

    TSharedPtr<FInternetAddr> Addr = SocketSub->GetAddressFromString(Host);
    if (!Addr.IsValid())
    {
        // Resolve hostname
        bool bIsValid = false;
        auto GAIResult = SocketSub->GetAddressInfo(*Host, nullptr, EAddressInfoFlags::Default, NAME_None);
        if (GAIResult.Results.Num() > 0)
        {
            // 첫 번째 결과를 사용 (대부분의 경우 충분)
            TSharedRef<FInternetAddr> Resolved = GAIResult.Results[0].Address;
            // 이후 Resolved를 사용하여 연결
            Addr = Resolved;
        }
    }

    if (Addr.IsValid())
    {
        Addr->SetPort(Port);
        bool bConnected = Socket->Connect(*Addr);
        if (!bConnected)
        {
            // For non-blocking sockets, Connect may return false but still be in progress; that's okay here.
        }
        return true;
    }

    return false;
}

void FTLSConnection::Close()
{
    IsConnect = false;
    UE_LOG(LogTemp, Log, TEXT("Set Is Not Connect"));

    IsRunning = false;
    if (WakeEvent) WakeEvent->Trigger();

    if (Thread)
    {
        Thread->Kill(true);
        delete Thread;
        Thread = nullptr;
    }

    if (Socket)
    {
        Socket->Close();
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
        Socket = nullptr;
        Tls.SetSocket(nullptr);
    }

    if (WakeEvent) { FPlatformProcess::ReturnSynchEventToPool(WakeEvent); WakeEvent = nullptr; }
}

// -----------------------------
// Usage notes (in this same doc):
// -----------------------------
// 1) Put TLSConnection.h in Public/ and TLSConnection.cpp in Private/ of your module folder.
// 2) Adjust Build.cs to find/link OpenSSL for each target platform. On Linux/macOS you can often link system openssl
//    (libssl, libcrypto). On Windows you may ship OpenSSL binaries in ThirdParty and add their lib paths.
// 3) This example uses a simple thread (FRunnableThread) that polls the socket. For higher performance
//    replace socket recv/send with platform-native async APIs (IOCP / epoll / io_uring / kqueue) and integrate
//    the memory-BIO TLS loop into their callbacks.
// 4) Certificate verification: SSL_CTX_load_verify_locations and SSL_set_verify must be used for production to
//    verify server certificates (this example omits for brevity).
// 5) Make sure to initialize OpenSSL appropriately in module startup if you need shared context or CA stores.

// End of document
