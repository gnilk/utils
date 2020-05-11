# utils
Set of standalone and portable functionality.

NOTE: xmlparser and logger have own repos.

- XMLParser, https://github.com/gnilk/xmlparser
- Logger, https://github.com/gnilk/logger


More or less each piece of code can be dropped into a project without external dependencies.
Some depend on C++ standard library most only on the C std library.

*NOTE:* Depends on https://github.com/gnilk/testrunner

Remove unit testing from CMakeLists.txt if you don't need them (this removes the dependency)