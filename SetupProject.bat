@echo on

cd FastBuild
call GenerateProjects.bat
call GenerateSolution.bat
cd ..
start FastBuild\CopyDll.bat
start FastBuild\GenerateStringID.bat
PAUSE