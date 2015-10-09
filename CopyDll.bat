@echo off
::Enter in the Build directory
cd Build
::Check if the x64-Debug exist

goto :check-existance

:: check if the directories exist, and if not, create it
:check-existance
  for %%X in (x64-Debug x64-Release x64-Retail x64-Profile) do (
    if EXIST %%X (
      echo %%X  already exist.
      ) else (
        echo %%X  does not exist so has been generated.
        mkdir %%X
      )
  )
  :: go next operation
  goto :copycontext

:: copy opengl and sdl from vendor to all the target
:copycontext
  for %%X in (x64-Debug x64-Release x64-Retail x64-Profile) do (
    xcopy /y ..\Vendors\OpenGL\lib\x64\*.dll %%X
  )
  :: go next operation
  goto :copyassimp

:: copy assimp dll from vendrors to the target
:copyassimp
  for %%X in (x64-Debug x64-Release x64-Retail x64-Profile) do (
    xcopy /y ..\Vendors\assimp\x64\Release\*.dll %%X
  )
  :: go next operation
  goto :copyphysx

:: copy assimp dll from vendors to the target
:copyphysx
:: debug vendors
  xcopy /y ..\Vendors\PhysX\Binaries\x64\Debug\*.dll x64-Debug
  for %%X in (x64-Release x64-Retail x64-Profile) do (
:: relese vendors
    xcopy /y ..\Vendors\PhysX\Binaries\x64\Release\*.dll %%X
  )
  :: go next operation
  goto :revert

:revert
::come back in the current directory
  echo End of script
  cd ..
  goto :eof
