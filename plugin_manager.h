#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include <vector>

struct PluginDescriptor {
    const char* name;
    int arity;
    double(__cdecl* func)(const double* args, int argc);
};

class PluginManager {
public:
    void loadPlugins(const std::string& directory);
    bool hasFunction(const std::string& name) const;
    double callFunction(const std::string& name, const std::vector<double>& args) const;
    void listFunctions() const;
private:
    std::unordered_map<std::string, PluginDescriptor> functions;
};
