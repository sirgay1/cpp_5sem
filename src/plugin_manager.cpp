#include "plugin_manager.h"
#include <windows.h>
#include <iostream>
#include <filesystem>
#include <algorithm>

using namespace std;
namespace fs = std::filesystem;

void PluginManager::loadPlugins(const string& directory) {
    functions.clear();

    if (!fs::exists(directory)) {
        cerr << "Plugins folder not found: " << directory << endl;
        return;
    }

    for (auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".dll") {
            string path = entry.path().string();
            HMODULE h = LoadLibraryA(path.c_str());
            if (!h) {
                cerr << "Failed to load plugin: " << path << endl;
                continue;
            }

            auto reg = (PluginDescriptor * (__cdecl*)())GetProcAddress(h, "register_plugin");
            if (!reg) {
                cerr << "register_plugin not found in " << path << endl;
                FreeLibrary(h);
                continue;
            }

            try {
                PluginDescriptor* desc = reg();
                if (desc) {
                    string name = desc->name ? desc->name : "";
                    transform(name.begin(), name.end(), name.begin(), ::tolower);
                    functions[name] = *desc;
                    cout << "Loaded plugin: " << name
                        << " (arity " << desc->arity << ")" << endl;
                }
            }
            catch (const exception& e) {
                cerr << "Error in plugin " << path << ": " << e.what() << endl;
                FreeLibrary(h);
            }
        }
    }

    if (functions.empty())
        cerr << "Warning: no plugins loaded from " << directory << endl;
}

bool PluginManager::hasFunction(const string& name) const {
    string key = name;
    transform(key.begin(), key.end(), key.begin(), ::tolower);
    return functions.find(key) != functions.end();
}

double PluginManager::callFunction(const string& name,
    const vector<double>& args) const {
    string key = name;
    transform(key.begin(), key.end(), key.begin(), ::tolower);
    auto it = functions.find(key);
    if (it == functions.end())
        throw runtime_error("Unknown function: " + name);

    const auto& f = it->second;
    if ((int)args.size() != f.arity)
        throw runtime_error("Function '" + name + "' expects " +
            to_string(f.arity) + " arguments");

    try {
        return f.func(args.data(), (int)args.size());
    }
    catch (const exception& e) {
        throw runtime_error("Error in function '" + name + "': " + e.what());
    }
}

void PluginManager::listFunctions() const {
    cout << "Available functions: ";
    for (auto& [name, _] : functions)
        cout << name << " ";
    cout << endl;
}
