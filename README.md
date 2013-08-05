HTTP Daemon Plugins
===================
Plugins for the http-daemon

Plugins should be linked to the http-daemon-components library and should
respect [these regulations](https://qt-project.org/doc/qt-4.8/plugins-howto.html#the-lower-level-api-extending-qt-applications)
in order to be loadable by the daemon.

In order to do this the paths in the project file must be changed to point to
the header files of the http-daemon-components and to the library provided by
the same project.

The qmake variables that need to be changed are: `LIBS`, `INCLUDEPATH`,
`DEPENDPATH`, `PRE_TARGETDEPS`.

After compilation, the plugins (in the form of dynamic libraries) should be
copied in the directory set via the command line argument given to the
http-daemon when started (`--pluginroot`) so that the daemon can find and load
them.

License
=======
Copyright 2013 Barbu Paul - Gheorghe

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
