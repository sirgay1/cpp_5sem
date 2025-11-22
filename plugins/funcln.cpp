// funcln.cpp
// Компилировать в DLL: cl /LD /std:c++17 funcln.cpp /Fe:funcln.dll
// Или: g++ -shared -o funcln.dll funcln.cpp -Wl,--out-implib,libfuncln.a

#include <cmath>
#include <stdexcept>

extern "C" {

    struct PluginDescriptor {
        const char* name;
        int arity;
        double(__cdecl* func)(const double* args, int argc);
    };

    static double __cdecl ln_impl(const double* args, int argc) {
        if (argc != 1) throw std::runtime_error("ln expects 1 argument");
        double x = args[0];
        if (x <= 0.0) throw std::runtime_error("ln domain error: argument must be > 0");
        return std::log(x);
    }

    static PluginDescriptor descriptor = {
        "ln",
        1,
        ln_impl
    };

    __declspec(dllexport) PluginDescriptor* __cdecl register_plugin() {
        return &descriptor;
    }

} // extern "C"
