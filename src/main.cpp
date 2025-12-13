#include "plugin_manager.h"
#include "calculator.h"
#include <iostream>
#include <string>

int main() {
    PluginManager plugins;
    plugins.loadPlugins("./plugins");
    plugins.listFunctions();

    Calculator calc(plugins);

    std::string expr;
    std::cout << "Enter expression (empty to quit):\n";
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, expr);
        if (expr.empty()) break;

        try {
            double result = calc.evaluate(expr);
            std::cout << result << "\n";
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }
}
