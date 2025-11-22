# cpp_5sem
Запускать команду нужно через консоль. Если при запуске calc.exe не выводятся сообщения: 
Loaded plugin: ln (arity 1)
Loaded plugin: sin (arity 1)
Available functions: sin ln
То это значит, что обе либо какая-то из функций не подключена. Для подключения нужно в консоле перейти в папку /plugins и подключить dll следующиим способом: g++ -shared -std=c++17 funcsin.cpp -o funcsin.dll -Wl,--out-implib,libfuncsin.a      - для sin
          g++ -shared -std=c++17 funcln.cpp -o funcln.dll -Wl,--out-implib,libfuncln.a         - для ln
Потом снова перейти в папку с calc.exe и запустить экзешник.
