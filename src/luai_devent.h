#ifndef LUAI_DEVENT_H
#define LUAI_DEVENT_H
#include "lua.h"
#include "lj_dispatch.h"

void callhook(lua_State *L, int event, BCLine line);
#define LUA_CALLHOOK(L,event, type) callhook(L, event, type)
#define LUA_S2V(s) (s)

#define luai_threadevent(L, from, type)         \
    if (L && (G(L)->hookmask2 & LUA_MASKTHREAD)) {  \
        setpvalue(LUA_S2V(L->top), from);       \
        L->top++;                               \
        LUA_CALLHOOK(L, LUA_HOOKTHREAD, type);  \
        L->top--;                               \
    }

#define luai_threadcall(L, from) luai_threadevent(L, from, 0)
#define luai_threadret(L, to) luai_threadevent(L, to, 1)

#define LUA_ERREVENT_PANIC 0x10

#define luai_errevent_(L, errcode) LUA_CALLHOOK(L, LUA_HOOKEXCEPTION, errcode)

#define luai_errevent(L, errcode)           \
    if (G(L)->hookmask2 & LUA_MASKEXCEPTION) {  \
        switch (errcode) {                  \
        case LUA_ERRRUN:                    \
        case LUA_ERRSYNTAX:                 \
        case LUA_ERRMEM:                    \
        case LUA_ERRERR: {                  \
            int code = errcode;             \
            code |= LUA_ERREVENT_PANIC;     \
            luai_errevent_(L, code);        \
            break;                          \
        }}                                  \
    }
#endif