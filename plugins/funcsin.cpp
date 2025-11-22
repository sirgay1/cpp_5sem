#include <cmath>
#include <stdexcept>

extern "C" {

    struct PluginDescriptor {
        const char* name;
        int arity;
        double(__cdecl* func)(const double*, int);
    };
    
    static double __cdecl sin_deg(const double* args, int argc) {
        const double pi = std::acos(-1);
        if (argc != 1) throw std::runtime_error("sin expects 1 argument");
        double radians = args[0] * pi / 180.0;
        return std::sin(radians);
    }

    static PluginDescriptor desc = { 
        "sin", 
        1, 
        sin_deg
    };

    __declspec(dllexport) PluginDescriptor* __cdecl register_plugin() {
        return &desc;
    }

}
