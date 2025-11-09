#include "bridge_lua.h"
#include <iostream>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

// Basic registry of callbacks - stored in Lua registry as lightuserdata + ref mapping.
// For simplicity we'll use a static map keyed by integer ref, and expose a C function
// that Lua can call with the ref and a string.

#include <map>

static std::map<int, std::function<void(const std::string&)>> g_callbacks;
static std::mutex g_cb_mtx;

static int lua_invoke_registered_callback(lua_State* L) {
    // stack: ref (number), payload (string)
    int ref = (int)lua_tointeger(L, 1);
    const char* payload = lua_tostring(L, 2);
    std::string s = payload ? payload : "";
    std::function<void(const std::string&)> cb;
    {
        std::lock_guard<std::mutex> lock(g_cb_mtx);
        auto it = g_callbacks.find(ref);
        if (it != g_callbacks.end()) cb = it->second;
    }
    if (cb) cb(s);
    return 0;
}

BridgeLua::BridgeLua() : L(nullptr), next_cb_ref(1) {
    L = luaL_newstate();
    if (L) {
        luaL_openlibs(L);
        // register invocation function
        lua_register(L, "cpp_invoke_cb", lua_invoke_registered_callback);
    }
}

BridgeLua::~BridgeLua() {
    {
        std::lock_guard<std::mutex> lock(g_cb_mtx);
        g_callbacks.clear();
    }
    if (L) lua_close(L);
}

bool BridgeLua::load_script(const std::string& path, std::string& err) {
    std::lock_guard<std::mutex> lock(mtx);
    if (!L) { err = "Lua state not initialized"; return false; }
    int r = luaL_dofile(L, path.c_str());
    if (r != LUA_OK) {
        const char* msg = lua_tostring(L, -1);
        err = msg ? msg : "unknown error";
        lua_pop(L, 1);
        return false;
    }
    return true;
}

std::optional<std::string> BridgeLua::call_global_stringfn(const std::string& func, const std::string& arg, std::string& err) {
    std::lock_guard<std::mutex> lock(mtx);
    if (!L) { err = "lua not initialized"; return std::nullopt; }
    lua_getglobal(L, func.c_str());
    if (!lua_isfunction(L, -1)) {
        lua_pop(L, 1);
        err = "function not found: " + func;
        return std::nullopt;
    }
    lua_pushlstring(L, arg.c_str(), arg.size());
    if (lua_pcall(L, 1, 1, 0) != LUA_OK) {
        const char* msg = lua_tostring(L, -1);
        err = msg ? msg : "lua error";
        lua_pop(L, 1);
        return std::nullopt;
    }
    if (lua_isstring(L, -1)) {
        size_t len;
        const char* s = lua_tolstring(L, -1, &len);
        std::string res(s, len);
        lua_pop(L, 1);
        return res;
    }
    lua_pop(L, 1);
    return std::nullopt;
}

std::optional<double> BridgeLua::call_global_numberfn(const std::string& func, std::string& err) {
    std::lock_guard<std::mutex> lock(mtx);
    if (!L) { err = "lua not initialized"; return std::nullopt; }
    lua_getglobal(L, func.c_str());
    if (!lua_isfunction(L, -1)) {
        lua_pop(L, 1);
        err = "function not found: " + func;
        return std::nullopt;
    }
    if (lua_pcall(L, 0, 1, 0) != LUA_OK) {
        const char* msg = lua_tostring(L, -1);
        err = msg ? msg : "lua error";
        lua_pop(L, 1);
        return std::nullopt;
    }
    if (lua_isnumber(L, -1)) {
        double v = lua_tonumber(L, -1);
        lua_pop(L, 1);
        return v;
    }
    lua_pop(L, 1);
    return std::nullopt;
}

void BridgeLua::register_callback(const std::string& name, std::function<void(const std::string&)> cb) {
    std::lock_guard<std::mutex> lock(g_cb_mtx);
    int ref = next_cb_ref++;
    g_callbacks[ref] = std::move(cb);
    // Expose the ref to Lua by setting a global constant with the callback id (optional)
    std::lock_guard<std::mutex> l(mtx);
    if (!L) return;
    lua_pushinteger(L, ref);
    lua_setglobal(L, name.c_str());
}
