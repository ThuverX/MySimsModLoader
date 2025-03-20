//
// Created by exozg on 19/03/2025.
//
#pragma once

#pragma pack(1)

// taken from https://gitlab.com/znixian/payday2-superblt/-/blob/master/platforms/w32/loader/wsock.cpp?ref_type=heads

#pragma region ALLFUNC(FUNC)

#define ALLFUNC(FUNC) \
        FUNC(0,__WSAFDIsSet,151) \
        FUNC(1,accept,1) \
        FUNC(2,AcceptEx,1141) \
        FUNC(3,bind,2) \
        FUNC(4,closesocket,3) \
        FUNC(5,connect,4) \
        FUNC(6,dn_expand,1106) \
        FUNC(7,EnumProtocolsA,1111) \
        FUNC(8,EnumProtocolsW,1112) \
        FUNC(9,GetAcceptExSockaddrs,1142) \
        FUNC(10,GetAddressByNameA,1109) \
        FUNC(11,GetAddressByNameW,1110) \
        FUNC(12,gethostbyaddr,51) \
        FUNC(13,gethostbyname,52) \
        FUNC(14,gethostname,57) \
        FUNC(15,GetNameByTypeA,1115) \
        FUNC(16,GetNameByTypeW,1116) \
        FUNC(17,getnetbyname,1101) \
        FUNC(18,getpeername,5) \
        FUNC(19,getprotobyname,53) \
        FUNC(20,getprotobynumber,54) \
        FUNC(21,getservbyname,55) \
        FUNC(22,getservbyport,56) \
        FUNC(23,GetServiceA,1119) \
        FUNC(24,GetServiceW,1120) \
        FUNC(25,getsockname,6) \
        FUNC(26,getsockopt,7) \
        FUNC(27,GetTypeByNameA,1113) \
        FUNC(28,GetTypeByNameW,1114) \
        FUNC(29,htonl,8) \
        FUNC(30,htons,9) \
        FUNC(31,inet_addr,10) \
        FUNC(32,inet_network,1100) \
        FUNC(33,inet_ntoa,11) \
        FUNC(34,ioctlsocket,12) \
        FUNC(35,listen,13) \
        FUNC(36,MigrateWinsockConfiguration,24) \
        FUNC(37,NPLoadNameSpaces,1130) \
        FUNC(38,ntohl,14) \
        FUNC(39,ntohs,15) \
        FUNC(40,rcmd,1102) \
        FUNC(41,recv,16) \
        FUNC(42,recvfrom,17) \
        FUNC(43,rexec,1103) \
        FUNC(44,rresvport,1104) \
        FUNC(45,s_perror,1108) \
        FUNC(46,select,18) \
        FUNC(47,send,19) \
        FUNC(48,sendto,20) \
        FUNC(49,sethostname,1105) \
        FUNC(50,SetServiceA,1117) \
        FUNC(51,SetServiceW,1118) \
        FUNC(52,setsockopt,21) \
        FUNC(53,shutdown,22) \
        FUNC(54,socket,23) \
        FUNC(55,TransmitFile,1140) \
        FUNC(56,WEP,500) \
        FUNC(57,WSAAsyncGetHostByAddr,102) \
        FUNC(58,WSAAsyncGetHostByName,103) \
        FUNC(59,WSAAsyncGetProtoByName,105) \
        FUNC(60,WSAAsyncGetProtoByNumber,104) \
        FUNC(61,WSAAsyncGetServByName,107) \
        FUNC(62,WSAAsyncGetServByPort,106) \
        FUNC(63,WSAAsyncSelect,101) \
        FUNC(64,WSACancelAsyncRequest,108) \
        FUNC(65,WSACancelBlockingCall,113) \
        FUNC(66,WSACleanup,116) \
        FUNC(67,WSAGetLastError,111) \
        FUNC(68,WSAIsBlocking,114) \
        FUNC(69,WSApSetPostRoutine,1000) \
        FUNC(70,WSARecvEx,1107) \
        FUNC(71,WSASetBlockingHook,109) \
        FUNC(72,WSASetLastError,112) \
        FUNC(73,WSAStartup,115) \
        FUNC(74,WSAUnhookBlockingHook,110) \

#define ALLFUNC_COUNT 75

#pragma endregion

FARPROC p[ALLFUNC_COUNT] = {nullptr};

void exports(HMODULE hL){
#define REGISTER(num, name, ordinal) p[num] = GetProcAddress(hL, #name);
	ALLFUNC(REGISTER);
#undef REGISTER
}

#define DEF_STUB(num, name, ordinal) \
extern "C" void _WSOCK_EXPORT_##name() { \
static void (*func)() = (void(*)())p[num]; \
func(); \
}
ALLFUNC(DEF_STUB)
#undef DEF_STUB
