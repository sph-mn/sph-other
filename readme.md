# emacs
* color-theme-sph: a color theme in three versions: dark, light, monochrome
* sph-scheme-mode: a simpler, faster, standalone emacs mode for editing scheme with regular indentation where indent depth corresponds to nesting depth
* sph-sc-mode: an emacs mode for editing [sc](https://github.com/sph-mn/sph-sc)

## sph-scheme-mode
* syntax highlighting and indentation
* regular indentation - every indentation step is represented by the same width
* highlighting of identifiers in definitions (with different colors for variable, procedure and syntax definitions)
* srfi-62 #;() s-exp comment support
* strategic syntax highlighting for only a few keywords per default: (if q l debug-log)

### versus scheme-mode (scheme.el)
* scheme-mode includes support for the dsssl-language and several additions for specific implementations
* lisp/scheme-mode indentation handling takes more than 500 lines, vs ~20 in this implementation using regular indentation
* ~600 lines to ~200
* sph-scheme-mode is much faster
* it is perhaps also minimal example code for learning how to write an emacs mode

### installation
move the .el file to a location you seem fitting, the user specific configuration directory like .emacs.d perhaps, or system-wide like /usr/share/emacs/site-lisp.
include it in your emacs configuration, $HOME/.emacs, with a line like (load-library \"~/path-to/sph-scheme-mode.elc\")"

add the following to your emacs config file

    (require (quote sph-scheme-mode))

when the file has been loaded, activate with M-x

    sph-scheme-mode

## color theme preview:
![color theme dark](emacs/color-theme/dark.png?raw=true)
![color theme monochrome](emacs/color-theme/monochrome.png?raw=true)

# scheme/fun
guile modules that depend on [sph-lib](https://github.com/sph-mn/sph-lib)

# hypercube projections
ga-hypercubes contains code for hypercube rotation animations in the browser and another readme
