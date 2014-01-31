Module skeleton generator for Magento. You can create a basic modules directory structure and apply an event observer or rewrite php class by making a few clicks. 

Usage: 
Point application to your Magento site root directory;
Enter app name and company name as well;
Click on appropriate checkboxes if you want to add blocks, model and helpers to your Magento module;
Rrewrite php class by selecting parent class from dropdown list and clicking "Apply" button;
Add an event oberver and click "Apply" button;
Click "Save" button to create new module skeleton inside your Magento site root directory;

MagentoTools created using QT C++ framework and can be compiled for different platforms. 


I would advice you to build QT framework from sources files (can be downloaded here http://qt-project.org/downloads). it makes sure to get framework builded and configured correctly. Also you will know that you have all required dependencies resolved on your system (some unresolved dependencies can cause problems during application building process). 

                                    To get QT builded statically (Ubuntu)
1. Download and extract qt source files to any folder you like.
2. cd to directory with extracted qt sources.
3. ./configure -static -release -qt-xcb -nomake examples -nomake tools -prefix /opt/Qt-static
4. make
5. make install
 
                                    To get QT builded statically (Windows)
1. install python and perl.
2. open qt cmd console tool.
3. cd to directory with extracted qt sources.
4. configure -static -release -opensource -opengl desktop -nomake examples -nomake tests -platform win32-g++ -prefix C:\Qt\Qt5.0.2-static
5. jom
6. jom install

Jom itself is qt version of make program. Using jom instead nmake is preferable in order to avoid problems during build. 

Replace -static parameter by -shared if you want to deploy your applications dynamically linked against other libraries.

            Add the following lines to you project.pro file in order to build application statically.

QMAKE_LFLAGS = -static -static-libgcc -static-libstdc++

CONFIG += release

CONFIG += static

