Calculator

При запуске main.cpp выведется:
```
Failed to load plugin: ./plugins\funcln.dll
Failed to load plugin: ./plugins\funcsin.dll
Warning: no plugins loaded from ./plugins
Available functions:
Enter expression (empty to quit):
>
```
Это значит что у нас не подключены dll, да и самого .exe у нас нет.

Для того, чтобы создать exe нужно в консоли перейти в нужную папку и написать:
"g++ -std=c++17 calculator.cpp plugin_manager.cpp main.cpp -o calc.exe"
По cpp файлам он создаст сам calc.exe.
Запустив его мы получим калькулятор без sin, ln и тд.

Чтобы добвить dll нужно перейти в папку /plugins в коносоли.
Далее скомпилировать dll файлы из cpp следующим способом:
"g++ -shared -std=c++17 funcsin.cpp -o funcsin.dll -Wl,--out-implib,libfuncsin.a"
Добавит funcsin.dll

"g++ -shared -std=c++17 funcln.cpp -o funcln.dll -Wl,--out-implib,libfuncln.a"
Добавит funcln.dll

После добавления dll файлов переходим в папку с calc.exe и при запуске видим:
```
Loaded plugin: ln (arity 1)
Loaded plugin: sin (arity 1)
Available functions: sin ln
Enter expression (empty to quit):
>
```
Значит dll файлы успешно загружены и калькулятор работает.
