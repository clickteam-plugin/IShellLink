IShellLink
==========

## Presentation

Provides `IShellLink` COM interface inside MMF/CF's applications. `IShellLink` interface allows to get and set Shell link file's parameters, load and save them from usual Shell link file (having `LNK` extension).

## Alternatives

Before creating this extension, I was using the following undocumented but well-known command line expression:

    appwiz.cpl,NewLinkHere
    
This one is not as handy and complete as `IShellLink` interface and behaves differently before and after _Microsoft Windows Vista_.

Some others extensions may allow to read and create Shell link file, perhaps _COM object_ for example.

## Details

IShellLink is currently developed using:
- MMF 2 standard SDK
- Microsoft Visual C++ 2005 Express Edition

Licensed under:
- Apache License, Version 2.0, http://www.apache.org/licenses/LICENSE-2.0

## References

1. http://blogs.msdn.com/b/vistacompatteam/archive/2006/10/20/rundll32-exe-appwiz-cpl-newlinkhere-is-back.aspx
