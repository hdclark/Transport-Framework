
=== Project - Transport ===

> "Either it'll move me,
>  or it'll move right through me.
>  Fully, and completely."
>
> -- Gord Downie, presumably reflecting on the law of conservation of momentum.

== Where to get started ==

The best place to learn about what this framework can do, consult the document:
    "References\_etc/Technical\_report\_1.pdf".
 To learn about the code structure, organization, or design, (currently) the best
 place to look is at the source itself (a future technical report will address
 these questions, but currently no documentation exists.)

== Introduction ==

This software simulates transport through a medium. It is not intended to be fast, but
 flexible. It does not (or will, hopefully) rely on specific techniques (like Monte
 Carlo) but will have a modular design to allow the use of various techniques in a 
 lego-like fashion.

Features:
 - Core code written in C++0x code.
 - "Everything is a module" approach is taken, providing extreme flexibility without 
    having to much around with the code, recompile, or compile everything with some
    funky compiler options.
 - Library modules are loaded at runtime. Program is run from the command line.
 - Lua scripting is (will be) embedded in modules, providing real-time adjustments if
    desired.

== History ==

This software was written from scratch[+] for homework from a course I took in early 2012.
 It simulates transport through a medium. It is designed to be modular and flexible 
 rather than fast. 

[+] Some source files were borrowed from previous projects and modified to suit. 
    As of writing, this is inclusively "Misc.h/cc" and "MyMath.h/cc".

== Design Choices ==

Various arrangements should be able to be 'tacked' onto the basic code. For instance,
 the initial particle should be able to be specified to be a photon, electron, positron,
 or whatever - but whatever is chosen, it should be separated from the code of the other
 types. In other words, one should be able to link in libraries as they are needed at 
 runtime. I believe this is the best way to ensure maximum flexibility for code that 
 does not need to mix.

For instance, if we are simulating heavy atoms propagating through water, and there is 
 a possibility of the lead atoms fracturing into two lighter elements, then we should 
 write each element independently and link them in as needed. Since photon interactions
 will probably not result in heavy atom production, we can selectively load in only the
 particle types we need at runtime. Having modules which load at runtime we are able to 
 simply specify the initial beam type via the command line or an input file, instead of
 having to tweak anything and possibly recompile. It also opens the door for scripting
 language embedding very easily - we can simply make alternative modules which have 
 some extra features like Lua embedding, prompting the user for input, or handling io in 
 a more string-friendly environment.
 
The major downside is that performance will most likely take a dive because we are no 
 longer to implicitly tell the compiler information about the functions which are loaded
 in at runtime. We can mitigate this to some degree with compiler and branch hinting, 
 but these will require a large amount of time to test.

== Data ==

Data for the media modules are from various sources. Some has been provided by Dr. Tony
 Popescu for the purpose of the assignment this software was written for. This data 
 was interpolated using cubic splines by myself. Sources for additional data are:

    - http://www.nist.gov/pml/data/xraycoef/index.cfm
        (Tables of X-Ray Mass Attenuation Coefficients and Mass Energy-Absorption 
         Coefficients from 1 keV to 20 MeV for Elements Z = 1 to 92 and 48 
         Additional Substances of Dosimetric Interest)

    - http://www.mathworks.com/matlabcentral/fileexchange/12092
        (PhotonAttenuation 2 Matlab package by Jaroslaw Tuszynski)

    - http://www.nist.gov/pml/data/xcom/index.cfm
        (XCOM: Photon Cross Sections Database)



