@echo off
setlocal enabledelayedexpansion

set ProjectName=Minimal Orca Application
set ProjectNameSafe=MinOrca

set SourceFolder=../src
set DataFolder=../data
set AppIconPath=%DataFolder%/icon.png
set WasmModulePath=module.wasm
set MainSourcePath=%SourceFolder%/main.c

for /f %%i in ('orca sdk-path') do set OrcaSdkPath=%%i

:: Main Flags
:: --target=wasm32 = Output to wasm32 binary
:: -mbuild-memory = Something about bulk memory operations like memset being opt-in instructions in wasm, this is how we make those functions compile to single instructions
:: -g = Generate debug symbols
:: -O2 = Optimization level 2
set CompilerFlags=--target=wasm32 -mbulk-memory -g -O2
:: Linker Flags
:: -Wl,--no-entry = Allow no entry point in this compilation (Orca will act as the entry point, and it will use our wasm binary when we do orca bundle)
:: -Wl,--export-dynamic = ?
set CompilerFlags=%CompilerFlags% -Wl,--no-entry -Wl,--export-dynamic
:: Include Directories
:: --sysroot = Set the include directory for standard library headers (like stdint.h)
:: -I = Add an include directory so search when resolving #include "..." lines
set CompilerFlags=%CompilerFlags% --sysroot %OrcaSdkPath%/orca-libc -I%OrcaSdkPath%/src -I%OrcaSdkPath%/src/ext
:: Linker Flags
:: -L = Add a lib include folder
:: -lorca_wasm = This is the precompiled binary that we compile with in order to get all the orca API functions exposed to us
set LinkerFlags=-L %OrcaSdkPath%/bin -lorca_wasm

:: +--------------------------------------------------------------+
:: |                     Compile Wasm Module                      |
:: +--------------------------------------------------------------+
echo [Compiling WASM Module...]
clang %CompilerFlags% %LinkerFlags% -o %WasmModulePath% %MainSourcePath%
IF %ERRORLEVEL% NEQ 0 (
	echo [Compilation Failed!]
	EXIT /B %ERRORLEVEL%
) ELSE (
	echo [Compilation Finished!]
)

:: +--------------------------------------------------------------+
:: |                   Bundle Orca Application                    |
:: +--------------------------------------------------------------+
orca bundle --name %ProjectNameSafe% --icon %AppIconPath% --resource-dir %DataFolder% %WasmModulePath%
