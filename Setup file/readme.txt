Note : Remember you need to compile the executable file with Release option, not Debug.

1 - In the first step you should develop you application. Then you need to compile it and get a Release executable 
  version of it.

2 - place the executable file in the "Final setup" folder along with the "DBFile" wich contains the database file.

3 - This executable file won't work without the dependency files. So we need to place the dependency files along with it.
  for that purpose you can use the following command in the proper terminal wich is installed by QT maintenance tool and
  it should be compatible with the compiler wich you compile your executable file. for example:

  if the compiler is ==> "QT 6.5.1 MSVC2019 64bit"    then we use ==> "Qt 6.5.1 (MSVC2019 64bit)" terminal

  the command we need to run in that terminal :
     windeployqt --no-translations --no-system-d3d-compiler  "Name of the file".exe

4 - Then you need to execute the setup.iss file in "Setup file" folder wich produces a installable file but you may need
  to configure it a little bit. 