#pragma once
// -----------------------------
// File: Public/TLSConnection.h
// -----------------------------

#include "CoreMinimal.h"

#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "IPAddress.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "UnrealEngineMessage.pb.h"
#include "Buffer.h"

#define UI OpenSSL_UI
# include "TLS.h"
#undef UI

/**
 * TLSConnection
 * - Cross-platform UE module example that uses FSocket for networking and OpenSSL memory BIO for TLS.
 * - Runs recv/send loop on a worker thread (FRunnable) to avoid blocking the game thread.
 * - This is a focused example: production code should add error handling, timeouts, reconnect logic,
 *   certificate validation, ALPN, session resumption, and resource cleanup robustness.
 */
class YUWIBO_API FTLSConnection : public FRunnable
{
public:
    FTLSConnection(TQueue<TUniquePtr<Buffer>>& _RecvBufQueue, TLS& tls);
    virtual ~FTLSConnection();

    // Initialize the connection: remote address and port
    bool Connect(const FString& Host, int32 Port);

    // Send plaintext data (will be encrypted then sent)

    bool TLSHandshake();
    bool FlushTLSWriteBuffer();

    // Graceful close
    void Close();

    // FRunnable
    virtual bool Init() override;
    virtual uint32 Run() override;
    virtual void Stop() override;
    virtual void Exit() override;

    bool GetIsConnect() { return IsConnect; }
    void WakeEventTrigger() { if(WakeEvent)WakeEvent->Trigger(); }
private:
    // socket
    FSocket* Socket;
    ISocketSubsystem* SocketSubsystem;
    FIPv4Endpoint RemoteEndpoint;

    // threaded run control
    FRunnableThread* Thread;
    FEvent* WakeEvent;

    // send queue
    FCriticalSection SendQueueMutex;
    TQueue<TUniquePtr<Buffer>> SendQueue;

    TLS& Tls;

    TQueue<TUniquePtr<Buffer>>& RecvBufQueue;
    Buffer RemainingData;

    FThreadSafeBool IsRunning = false;
    FThreadSafeBool IsDataRemaining = false;

    bool IsConnect = false;

    bool NonBlockingConnect(const FString& Host, int32 Port);
};